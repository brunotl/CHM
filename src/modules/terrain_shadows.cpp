
#include "terrain_shadows.hpp"

// Process has done i out of n rounds,
// and we want a bar of width w and resolution r.
//http://www.rosshemsley.co.uk/2011/02/creating-a-progress-bar-in-c-or-any-other-console-app/

static inline void loadbar(unsigned int x, unsigned int n, unsigned int w = 50)
{
    if ((x != n) && (x % (n / 100 + 1) != 0)) return;

    float ratio = x / (float) n;
    int c = ratio * w;

    std::cout << std::setw(3) << (int) (ratio * 100) << "% [";
    for (int x = 0; x < c; x++) std::cout << "=";
    for (int x = c; x < w; x++) std::cout << " ";
    std::cout << "]\r" << std::flush;
}

terrain_shadow::terrain_shadow(std::string ID)
{
    _provides->push_back("shadowed");
    _provides->push_back("z_prime");
    this->ID = ID;
    _parallel_type = parallel::domain;
    LOG_DEBUG << "Successfully instantiated module " << this->ID;

}

void terrain_shadow::run(mesh domain, boost::shared_ptr<global> global_param)
{
    double A = global_param->solar_az();
    double E = global_param->solar_el();

    if (global_param->solar_el() < 5)
    {
#pragma omp parallel for
        for (size_t i = 0; i < domain->size(); i++)
        {
            auto face = domain->face(i);
            face->set_face_data("z_prime", 0); //unshadowed
        }
        return;
    }
    //euler rotation matrix K
    arma::mat K;
    // eqns(6) & (7) in Montero
    double z0 = M_PI - A * M_PI / 180.0;
    double q0 = M_PI / 2.0 - E * M_PI / 180.0;

    K << cos(z0) << sin(z0) << 0 << arma::endr
            << -cos(q0) * sin(z0) << cos(q0) * cos(z0) << sin(q0) << arma::endr
            << sin(q0) * sin(z0) << -cos(z0) * sin(q0) << cos(q0) << arma::endr;


    //compute the rotation of each vertex

    //    tbb::concurrent_vector<triangulation::Face_handle> rot_faces;
    //    rot_faces.grow_by(domain->size());

#pragma omp parallel for
    for (size_t i = 0; i < domain->size(); i++)
    {
        auto face = domain->face(i);
        //for each vertex
        for (int j = 0; j < 3; j++)
        {
            if (!face->vertex(j)->info)
                face->vertex(j)->info = new vertex_flag();
            vertex_flag* vf = reinterpret_cast<vertex_flag*> (face->vertex(j)->info);
            if (!vf->visited)
            {
                arma::vec coord(3);
                triangulation::Point p;

                coord(0) = face->vertex(j)->point().x();
                coord(1) = face->vertex(j)->point().y();
                coord(2) = face->vertex(j)->point().z();

                coord = K*coord;
                p = triangulation::Point(coord(0), coord(1), coord(2));
                vf->prj_vertex = p;
                vf->org_vertex = face->vertex(j)->point();
                face->vertex(j)->set_point(vf->prj_vertex);

                vf->visited = true;
            }
        }
        //init memory but do nothing with it here
        module_shadow_face_info* tv = new module_shadow_face_info;
        face->info = tv;

        //save the iterator for the next step (sort))
        //        rot_faces.at(i) = face;
    }

    //modify the underlying triangulation to reflect the rotated vertices

    //    #pragma omp parallel for
    //    for(size_t i = 0; i<domain->size();i++)
    //    {
    //        auto face = domain->face(i);
    //        //we are iterating over each face, however a vertex may belong to
    //        // >1 face, so if we blindly modify a vertex, we may doubly or triply rotate a vertex.
    //        for (int j = 0; j < 3; j++)
    //        {
    //            if (!face->vertex(j)->info)
    //                BOOST_THROW_EXCEPTION(mesh_error() << errstr_info("Null vertex info"));
    //
    //            vertex_flag* vf = reinterpret_cast<vertex_flag*> (face->vertex(j)->info);
    //            
    //        }
    //
    // 
    //
    //        //save the iterator for the next step (sort))
    //        rot_faces.at(i) = face;
    //    }



    bounding_rect BBR;
    BBR.make(domain, 5, 5);

    #pragma omp parallel for
    for (size_t i = 0; i < domain->size(); i++)
    {
        mesh_elem t = domain->face(i);


        for (size_t j = 0; j < BBR.n_rows; j++)
        {
            for (size_t k = 0; k < BBR.n_cols; k++)
            {

                if (BBR.pt_in_rect(t->vertex(0)->point().x(), t->vertex(0)->point().y(), BBR.get_rect(j, k)) || //pt1
                        BBR.pt_in_rect(t->vertex(1)->point().x(), t->vertex(1)->point().y(), BBR.get_rect(j, k)) || //pt2
                        BBR.pt_in_rect(t->vertex(2)->point().x(), t->vertex(2)->point().y(), BBR.get_rect(j, k))) //pt3
                {
                    //t.shadow = j*k;
#pragma omp critical
                    {
                        BBR.get_rect(j, k)->triangles.push_back(t);
                    }
                }
            }

        }
    }

//    LOG_DEBUG << "AABB is " <<BBR.n_rows << "x" << BBR.n_rows;
//    for (size_t j = 0; j < BBR.n_rows; j++)
//    {
//        for (size_t k = 0; k < BBR.n_cols; k++)
//        {
//            LOG_DEBUG << BBR.get_rect(j, k)->triangles.size();
//        }
//    }

    
#pragma omp parallel for
    for (int i = 0; i < BBR.n_rows; i++)
    {
        for (int ii = 0; ii < BBR.n_cols; ii++)
        {
            //sort descending
            std::sort(BBR.get_rect(i, ii)->triangles.begin(), BBR.get_rect(i, ii)->triangles.end(),
                    [](triangulation::Face_handle fa, triangulation::Face_handle fb)->bool
                    {
                        return fa->center().z() > fb->center().z();
                    });

            size_t num_tri = BBR.get_rect(i, ii)->triangles.size();

            for (size_t j = 0; j < num_tri; j++)
            {
                //        loadbar(j, rot_faces.size());

                triangulation::Face_handle face_j = BBR.get_rect(i, ii)->triangles.at(j);

                K::Triangle_2 tj(K::Point_2(face_j->vertex(0)->point().x(), face_j->vertex(0)->point().y()),
                        K::Point_2(face_j->vertex(1)->point().x(), face_j->vertex(1)->point().y()),
                        K::Point_2(face_j->vertex(2)->point().x(), face_j->vertex(2)->point().y()));

                CGAL::Bbox_2 bj(tj.bbox());
                //compare to other triangles
                for (size_t k = j + 1; k < num_tri; k++)
                {
                    triangulation::Face_handle face_k = BBR.get_rect(i, ii)->triangles.at(k);
                    module_shadow_face_info* face_k_info = reinterpret_cast<module_shadow_face_info*> (face_k->info);
                    //face_k_info->shadow == 0 &&
                    if (face_j->get_z() > face_k->get_z()) //tj is above tk, and tk is shadded by tj?
                    {
                        K::Triangle_2 tk(K::Point_2(face_k->vertex(0)->point().x(), face_k->vertex(0)->point().y()),
                                K::Point_2(face_k->vertex(1)->point().x(), face_k->vertex(1)->point().y()),
                                K::Point_2(face_k->vertex(2)->point().x(), face_k->vertex(2)->point().y()));

                        CGAL::Bbox_2 bk(tk.bbox());

                        if (CGAL::do_overlap(bk, bj))
                        {
                            bool collision = face_k->intersects(face_j);
                            if (collision)
                            {

                                face_k_info->shadow = 1;
                            }
                        }
                    }
                }

            }
        }
    }

    // here we need to 'undo' the rotation we applied.
#pragma omp parallel for
    for (size_t i = 0; i < domain->size(); i++)
    {
        auto face = domain->face(i);

        face->set_face_data("z_prime", face->center().z());

        module_shadow_face_info* face_info = reinterpret_cast<module_shadow_face_info*> (face->info);
        face->set_face_data("shadowed", face_info->shadow);


        for (int i = 0; i < 3; i++)
        {
            if (!face->vertex(i)->info)
                BOOST_THROW_EXCEPTION(mesh_error() << errstr_info("Null vertex info"));

            vertex_flag* vf = reinterpret_cast<vertex_flag*> (face->vertex(i)->info);
            face->vertex(i)->set_point(vf->org_vertex);
            vf->visited = false; // for the next timestep!
        }

    }
}

terrain_shadow::~terrain_shadow()
{


}



