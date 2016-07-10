# FindLibUSB.cmake - Try to find the Hiredis library
# Once done this will define
#
#  LIBUSB_FOUND - System has libusb
#  LIBUSB_INCLUDE_DIR - The libusb include directory
#  LIBUSB_LIBRARY - The libraries needed to use libusb
#  LIBUSB_DEFINITIONS - Compiler switches required for using libusb
#
#  Original from https://github.com/texane/stlink/blob/master/cmake/modules/FindLibUSB.cmake

if(WIN32)
	set(LIBUSB_WIN_ARCHIVE libusb-1.0.20.7z)
	set(LIBUSB_WIN_ARCHIVE_PATH ${CMAKE_BINARY_DIR}/${LIBUSB_WIN_ARCHIVE})
	set(LIBUSB_WIN_OUTPUT_FOLDER ${CMAKE_BINARY_DIR}/libusb-1.0.20)

	if(EXISTS ${LIBUSB_WIN_ARCHIVE_PATH})
		message(STATUS "libusb archive already in build folder")
	else()
		file(DOWNLOAD
			https://sourceforge.net/projects/libusb/files/libusb-1.0/libusb-1.0.20/libusb-1.0.20.7z/download
			${LIBUSB_WIN_ARCHIVE_PATH}
			SHOW_PROGRESS
		)
	endif()
  
	execute_process(COMMAND ${ZIP_LOCATION} x -y ${LIBUSB_WIN_ARCHIVE_PATH} -o${LIBUSB_WIN_OUTPUT_FOLDER}
	)
endif()

FIND_PATH(LIBUSB_INCLUDE_DIR NAMES libusb.h
	HINTS
	/usr
	/usr/local
	/opt
	${LIBUSB_WIN_OUTPUT_FOLDER}/include
	PATH_SUFFIXES libusb-1.0
)

if (APPLE)
	set(LIBUSB_NAME libusb-1.0.a)
elseif(WIN32)
	set(LIBUSB_NAME libusb-1.0.lib)
else()
	set(LIBUSB_NAME usb-1.0)
endif()

FIND_LIBRARY(LIBUSB_LIBRARY NAMES ${LIBUSB_NAME}
	HINTS
	/usr
	/usr/local
	/opt
	${LIBUSB_WIN_OUTPUT_FOLDER}/MS32/static
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Libusb DEFAULT_MSG LIBUSB_LIBRARY LIBUSB_INCLUDE_DIR)

MARK_AS_ADVANCED(LIBUSB_INCLUDE_DIR LIBUSB_LIBRARY)