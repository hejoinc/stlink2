configure_file (
	"${CMAKE_CURRENT_SOURCE_DIR}/stlink2/build.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/stlink2/build.h"
)

file(GLOB STLINK2_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/stlink2/*.h")
set (STLINK2_HEADERS "${CMAKE_BINARY_DIR}/include/stlink2/build.h;${STLINK2_HEADERS}" CACHE INTERNAL "stlink2 headers")
install (
	FILES ${STLINK2_HEADERS}
	DESTINATION include/stlink2
)

install (
	FILES "${CMAKE_CURRENT_SOURCE_DIR}/stlink2.h"
	DESTINATION include
)
