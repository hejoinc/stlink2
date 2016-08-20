if (NOT BUILD_TESTING)
	message(STATUS "tests disabled")
	return()
endif()
message(STATUS "tests enabled")

set(TESTS
	log
	open_close
)

foreach(test ${TESTS})
	add_executable(${test} ${test}.c)
	add_dependencies(${test} ${STLINK2_LIBRARY_STATIC})
	target_link_libraries(${test} ${STLINK2_LIBRARY_STATIC})
	add_test(NAME "${test}" COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${test}")
endforeach()
