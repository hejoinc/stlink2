#
# Copyright 2016 Jerry Jacobs. All rights reserved.
# Use of this source code is governed by the MIT
# license that can be found in the LICENSE file.
#
cmake_minimum_required(VERSION 2.8)
include(CheckFunctionExists)

if (POLICY CMP0042)
	# Newer cmake on MacOS should use @rpath
	cmake_policy (SET CMP0042 NEW)
endif ()

project(stlink2 C)

set(STLINK2_VERSION_MAJOR 0)
set(STLINK2_VERSION_MINOR 0)
set(STLINK2_VERSION_PATCH 0)
set(STLINK2_VERSION "${STLINK2_VERSION_MAJOR}.${STLINK2_VERSION_MINOR}.${STLINK2_VERSION_PATCH}")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(WIN32)
	find_package(7Zip REQUIRED)
	message(STATUS "7Zip Location: " ${ZIP_LOCATION})
endif()

include(cmake/CheckCCompilerFlag.txt)
if (NOT WIN32)
	include(cmake/Lint.txt)
endif()

if (WIN32)
	add_definitions("-DSTLINK2_HAVE_WINDOWS")
	add_definitions("-Dsnprintf=_snprintf")
	add_definitions("-D__func__=__FUNCTION__") # MSVC doesn't support C99
	add_definitions("-D_WINSOCKAPI_")
	add_cflag_if_supported("/W4")
else ()
	check_function_exists(nanosleep STLINK2_HAVE_NANOSLEEP)
	add_definitions (-D_GNU_SOURCE)
	add_cflag_if_supported("-Wall")

	if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
		add_cflag_if_supported("-O0")
		add_cflag_if_supported("-ggdb")
		add_cflag_if_supported("-fprofile-arcs")
		add_cflag_if_supported("-ftest-coverage")
		add_cflag_if_supported("-fno-omit-frame-pointer")

		set(STLINK2_VERSION "${STLINK2_VERSION}-debug")
		add_definitions("-DSTLINK2_DEBUG")
		include(CTest)
	else()
		add_cflag_if_supported("-O2")
		add_cflag_if_supported("-Werror")
		add_cflag_if_supported("-fomit-frame-pointer")
		add_cflag_if_supported("-fvisibility=hidden")
	endif()
endif()

if(MSVC)
	add_cflag_if_supported("/MD")
#    foreach(flag_var CMAKE_C_FLAGS_DEBUG CMAKE_CXX_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS_RELWITHDEBINFO) 
#        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
#        string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
#    endforeach(flag_var)
#    SET (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}" CACHE STRING "MSVC C Debug MT flags " FORCE)    
#    SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "MSVC CXX Debug MT flags " FORCE)
#    SET (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "MSVC C Release MT flags " FORCE)
#    SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "MSVC CXX Release MT flags " FORCE)
#    SET (CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}" CACHE STRING "MSVC C Debug MT flags " FORCE)    
#    SET (CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}" CACHE STRING "MSVC C Release MT flags " FORCE)
#    SET (CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}" CACHE STRING "MSVC CXX Debug MT flags " FORCE)    
#    SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}" CACHE STRING "MSVC CXX Release MT flags " FORCE)
endif()

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")

add_subdirectory(include)
include_directories(include ${CMAKE_BINARY_DIR}/include)
add_subdirectory(src)

###
# Lint
###
if (BUILD_TESTING)
	add_lint_target("${STLINK2_HEADERS};${STLINK2_LIBRARY_SOURCE}")
endif()

add_subdirectory("tests")
