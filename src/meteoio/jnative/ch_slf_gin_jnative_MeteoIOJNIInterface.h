/* * Canadian Hydrological Model - The Canadian Hydrological Model (CHM) is a novel
 * modular unstructured mesh based approach for hydrological modelling
 * Copyright (C) 2018 Christopher Marsh
 *
 * This file is part of Canadian Hydrological Model.
 *
 * Canadian Hydrological Model is free software: you can redistribute it and/or
 * modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Canadian Hydrological Model is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Canadian Hydrological Model.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/*
 * ch_slf_gin_jnative_MeteoIOJNIInterface.h
 *
 *  Created on: 08.01.2010
 *      Author: perot
 */
/**
 * Header file generated by javah tool.
 * Make it possible to call the function marked as JNIEXPORT from any java API or application,
 * via JNI framework.
 *
 * The Java API or application must contains the interface :
 * */
/*package ch.slf.gin.jnative;

import java.lang.System;

public final class MeteoIOJNIInterface {

	static {
		System.loadLibrary(MeteoIOJNative.NATIVE_LIBRARY_PATH);
	}
	public static final native double[] executeInterpolationSubDem(
			String algorithm,
			String iointerface,
			String demFile,
			String demCoordSystem,
			double demXll,
			double demYll,
			double demXur,
			double demYur,
			double[] metadata,
			double[] data,
			String metaCoordSystem,
			String cellOrder
			);
	public static final native double[] executeInterpolation(
			String algorithm,
			String iointerface,
			String demFile,
			String demCoordSystem,
			double[] metadata,
			double[] data,
			String metaCoordSystem,
			String cellOrder
			);

	public static final native double[] executeClusterization(
			String algorithm,
			String iointerface,
			String demFile,
			String demCoordSystem,
			double[] metadata,
			double[] data,
			String metaCoordSystem,
			String cellOrder,
			double[] clusterThresholds,
			double[] clusterValues
			);


	public static final native double[] polygonize(
			String algorithm,
			String iointerface,
			String demFile,
			String demCoordSystem,
			double[] metadata,
			double[] data,
			String metaCoordSystem,
			String cellOrder,
			double[] clusterThresholds,
			double[] clusterValues,
			double[] options
			);

}*/

/**
 * TO compile :
 * 1) define preprocessor _METEOIO_JNI
 * 2) add path to JNI natives header
 * 	  -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32"
 * 3) add option "--add-stdcall-alias" to the linker, otherwise JNI couldn't make the relation
 *    between Java  and C++ function names
 *
 *   g++ -Xlinker --add-stdcall-alias -shared ch_slf_gin_jnative_MeteoIOJNIInterface.o
 *
 */

#ifdef _METEOIO_JNI


/* DO NOT EDIT THIS FILE - it is machine generated */
#include "jni.h"
/* Header for class ch_slf_gin_jnative_MeteoIOJNIInterface */

#ifndef _Included_ch_slf_gin_jnative_MeteoIOJNIInterface
#define _Included_ch_slf_gin_jnative_MeteoIOJNIInterface
#ifdef __cplusplus
extern "C" {
#endif
	/*
	 * Class:     ch_slf_gin_jnative_MeteoIOJNIInterface
	 * Method:    executeInterpolationSubDem
	 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;DDDD[D[DLjava/lang/String;Ljava/lang/String;)[D
	 */
	JNIEXPORT jdoubleArray JNICALL Java_ch_slf_gin_jnative_MeteoIOJNIInterface_executeInterpolationSubDem
	  (JNIEnv *, jclass, jstring, jstring, jstring, jstring, jdouble, jdouble, jdouble, jdouble, jdoubleArray, jdoubleArray, jstring, jstring);

	/*
	 * Class:     ch_slf_gin_jnative_MeteoIOJNIInterface
	 * Method:    executeInterpolation
	 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[D[DLjava/lang/String;Ljava/lang/String;)[D
	 */
	JNIEXPORT jdoubleArray JNICALL Java_ch_slf_gin_jnative_MeteoIOJNIInterface_executeInterpolation
	  (JNIEnv *, jclass, jstring, jstring, jstring, jstring, jdoubleArray, jdoubleArray, jstring, jstring);

	/*
	 * Class:     ch_slf_gin_jnative_MeteoIOJNIInterface
	 * Method:    executeClusterization
	 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[D[DLjava/lang/String;Ljava/lang/String;[D[D)[D
	 */
	JNIEXPORT jdoubleArray JNICALL Java_ch_slf_gin_jnative_MeteoIOJNIInterface_executeClusterization
	  (JNIEnv *, jclass, jstring, jstring, jstring, jstring, jdoubleArray, jdoubleArray, jstring, jstring, jdoubleArray, jdoubleArray);

	/*
	 * Class:     ch_slf_gin_jnative_MeteoIOJNIInterface
	 * Method:    polygonize
	 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[D[DLjava/lang/String;Ljava/lang/String;[D[D[D)[D
	 */
	JNIEXPORT jdoubleArray JNICALL Java_ch_slf_gin_jnative_MeteoIOJNIInterface_polygonize
	  (JNIEnv *, jclass, jstring, jstring, jstring, jstring, jdoubleArray, jdoubleArray, jstring, jstring, jdoubleArray, jdoubleArray, jdoubleArray);

#ifdef __cplusplus
}
#endif
#endif

#endif//_METEOIO_JNI