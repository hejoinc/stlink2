cmake_minimum_required(VERSION 2.8)
include(CheckFunctionExists)

if (POLICY CMP0042)
	# Newer cmake on MacOS should use @rpath
	cmake_policy (SET CMP0042 NEW)
endif ()

project(libstlink2 C)

#
# The path for our own CMake modules
#
set ( CMAKE_MODULE_PATH
	${PROJECT_SOURCE_DIR}/cmake
)

if(WIN32)
	find_package(7Zip REQUIRED)
	message(STATUS "7Zip Location: " ${ZIP_LOCATION})
endif()

find_package(LibUSB REQUIRED)
message(STATUS "libusb inc: " ${LIBUSB_INCLUDE_DIR})
message(STATUS "libusb lib: " ${LIBUSB_LIBRARY})

include(cmake/CheckCCompilerFlag.txt)
if(WIN32)
	function(add_lint SOURCES_LIST)
		message(STATUS "Linting not supported on WINx platforms")
	endfunction()
else()
	include(cmake/Lint.txt)
endif()

add_definitions(-D_GNU_SOURCE)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	add_cflag_if_supported("-O0")
	add_cflag_if_supported("-ggdb")
	add_cflag_if_supported("-fprofile-arcs")
	add_cflag_if_supported("-ftest-coverage")

	add_definitions("-DDEBUG")

	include(CTest)
else()
	add_cflag_if_supported("-O2")
	add_cflag_if_supported("-Werror")

	add_definitions("-DRELEASE")
endif()

check_function_exists(nanosleep HAVE_NANOSLEEP)

set(SRC
	src/stlink2.c
	src/usb.c
	src/stm32.c
)
include_directories(include)
include_directories(${LIBUSB_INCLUDE_DIR})

if(OCA_LINT AND BUILD_TESTING)
	add_lint(${SRC})
endif()

add_library(${PROJECT_NAME}-static STATIC ${SRC})
set_target_properties(${PROJECT_NAME}-static PROPERTIES OUTPUT_NAME ${PROJECT_NAME} PREFIX "")
target_link_libraries(${PROJECT_NAME}-static  ${LIBUSB_LIBRARY})

add_library(${PROJECT_NAME}-shared SHARED ${SRC})
set_target_properties(${PROJECT_NAME}-shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME} PREFIX "")
target_link_libraries(${PROJECT_NAME}-shared ${LIBUSB_LIBRARY})

if (APPLE)
	find_library(ObjC objc)
	find_library(CoreFoundation CoreFoundation)
	find_library(IOKit IOKit)
	target_link_libraries(${PROJECT_NAME}-static ${CoreFoundation} ${IOKit} ${ObjC})
	target_link_libraries(${PROJECT_NAME}-shared ${CoreFoundation} ${IOKit} ${ObjC})
endif()

add_subdirectory("tests")
