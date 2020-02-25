//
// Canadian Hydrological Model - The Canadian Hydrological Model (CHM) is a novel
// modular unstructured mesh based approach for hydrological modelling
// Copyright (C) 2018 Christopher Marsh
//
// This file is part of Canadian Hydrological Model.
//
// Canadian Hydrological Model is free software: you can redistribute it and/or
// modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Canadian Hydrological Model is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Canadian Hydrological Model.  If not, see
// <http://www.gnu.org/licenses/>.
//

#include "metdata.hpp"
#include "gtest/gtest.h"
#include <vector>
class MetdataTest : public testing::Test
{
  protected:

    virtual void SetUp()
    {
        logging::core::get()->set_logging_enabled(false);

    }
};

//basic default init sanity checks
TEST_F(MetdataTest, LoadAsciiData)
{
    metdata md("+proj=utm +zone=8 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs ");

    metdata::ascii_metdata station;
    station.path = "test_met_data_longer1.txt";
    station.latitude = 60.56726;
    station.longitude =  -135.184652;
    station.elevation = 1559;

    std::vector<metdata::ascii_metdata> s;
    s.push_back(station);

    ASSERT_NO_THROW(md.load_from_ascii(s,-8));

}

TEST_F(MetdataTest, ASCII_TwoStationDuplicatedID)
{
    metdata md("+proj=utm +zone=8 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs ");

    metdata::ascii_metdata station;
    station.path = "test_met_data_longer1.txt";
    station.latitude = 60.56726;
    station.longitude =  -135.184652;
    station.elevation = 1559;
    station.id = "station1";

    metdata::ascii_metdata station2;
    station2.path = "test_met_data_longer2.txt";
    station2.latitude = 60.56726;
    station2.longitude =  -135.184652;
    station2.elevation = 1559;
    station2.id = "station1";

    std::vector<metdata::ascii_metdata> s;

    s.push_back(station2);
    s.push_back(station);

    ASSERT_ANY_THROW(md.load_from_ascii(s,-8));

}

TEST_F(MetdataTest, ASCII_TwoStation)
{
    metdata md("+proj=utm +zone=8 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs ");

    metdata::ascii_metdata station;
    station.path = "test_met_data_longer1.txt";
    station.latitude = 60.56726;
    station.longitude =  -135.184652;
    station.elevation = 1559;
    station.id = "station1";

    metdata::ascii_metdata station2;
    station2.path = "test_met_data_longer2.txt";
    station2.latitude = 60.56726;
    station2.longitude =  -135.184652;
    station2.elevation = 1559;
    station2.id = "station2";

    std::vector<metdata::ascii_metdata> s;

    s.push_back(station2);
    s.push_back(station);

    ASSERT_NO_THROW(md.load_from_ascii(s,-8));

}
TEST_F(MetdataTest, ASCII_StartEndTime)
{
    metdata md("+proj=utm +zone=8 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs ");

    metdata::ascii_metdata station;
    station.path = "test_met_data_longer1.txt";
    station.latitude = 60.56726;
    station.longitude =  -135.184652;
    station.elevation = 1559;
    station.id = "station1";

    std::vector<metdata::ascii_metdata> s;
    s.push_back(station);


    ASSERT_NO_THROW(md.load_from_ascii(s,-8));

    auto start_time = md.start_time();
    auto end_time = md.end_time();

    auto s_true = boost::posix_time::from_iso_string("20101001T090000");
    auto e_true = boost::posix_time::from_iso_string("20101001T120000");
    ASSERT_EQ(start_time,s_true);
    ASSERT_EQ(end_time,e_true);

}


TEST_F(MetdataTest, ASCII_TwoStationStartEndTime)
{
    metdata md("+proj=utm +zone=8 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs ");

    metdata::ascii_metdata station;
    station.path = "test_met_data_longer1.txt";
    station.latitude = 60.56726;
    station.longitude =  -135.184652;
    station.elevation = 1559;
    station.id = "station1";

    metdata::ascii_metdata station2;
    station2.path = "test_met_data_longer2.txt";
    station2.latitude = 60.56726;
    station2.longitude =  -135.184652;
    station2.elevation = 1559;
    station2.id = "station2";

    std::vector<metdata::ascii_metdata> s;
    s.push_back(station);
    s.push_back(station2);

    ASSERT_NO_THROW(md.load_from_ascii(s,-8));

    boost::posix_time::ptime start_time,end_time;
    std::tie(start_time, end_time) = md.start_end_time();

    auto s_true = boost::posix_time::from_iso_string("20101001T100000");
    auto e = boost::posix_time::from_iso_string("20101001T120000");
    ASSERT_EQ(start_time,s_true);
    ASSERT_EQ(end_time,e);

}
TEST_F(MetdataTest, ASCII_TestCurrentTime)
{
    metdata md("+proj=utm +zone=8 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs ");

    metdata::ascii_metdata station;
    station.path = "test_met_data_longer1.txt";
    station.latitude = 60.56726;
    station.longitude = -135.184652;
    station.elevation = 1559;
    station.id = "station1";

    std::vector<metdata::ascii_metdata> s;
    s.push_back(station);

    ASSERT_NO_THROW(md.load_from_ascii(s, -8));

    auto cur_time = md.current_time_str();

    ASSERT_EQ(cur_time,"20101001T090000");
}

TEST_F(MetdataTest, ASCII_TestNext)
{
    metdata md("+proj=utm +zone=8 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs ");

    metdata::ascii_metdata station;
    station.path = "test_met_data_longer1.txt";
    station.latitude = 60.56726;
    station.longitude = -135.184652;
    station.elevation = 1559;
    station.id = "station1";

    metdata::ascii_metdata station2;
    station2.path = "test_met_data_longer2.txt";
    station2.latitude = 60.56726;
    station2.longitude = -135.184652;
    station2.elevation = 1559;
    station2.id = "station2";

    std::vector<metdata::ascii_metdata> s;

    s.push_back(station2);
    s.push_back(station);

    ASSERT_NO_THROW(md.load_from_ascii(s, -8));

    std::string cur_time = "";
    //returns false when no ts left

    bool done = !md.next();
    //we should not be done yet
    ASSERT_EQ(done,false);

    cur_time = md.current_time_str();
    ASSERT_EQ(cur_time,"20101001T110000");

    done = !md.next();
    ASSERT_EQ(done,false);
    cur_time = md.current_time_str();
    ASSERT_EQ(cur_time,"20101001T120000");

    done = !md.next();
    ASSERT_EQ(done,true);
    cur_time = md.current_time_str();
    ASSERT_EQ(cur_time,"20101001T130000");


}