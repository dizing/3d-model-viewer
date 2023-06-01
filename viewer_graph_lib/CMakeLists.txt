set(LIB_NAME "viewer_graph_lib")
file(GLOB SRC_FILES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/${LIB_NAME}/src/*.c)

file(GLOB TEST_FILES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/${LIB_NAME}/test/*.c)

add_library(${LIB_NAME} ${SRC_FILES})
target_include_directories(${LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/${LIB_NAME}/include)
# target_link_libraries(${LIB_NAME}
#   PUBLIC $<$<PLATFORM_ID:Linux>:m>)

find_package(Check)


if(Check_FOUND)
add_executable(test ${TEST_FILES})
target_link_libraries(test PUBLIC ${LIB_NAME} ${CHECK_LIBRARIES})
target_include_directories(test PRIVATE ${CHECK_INCLUDE_DIR})
endif()

if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_BUILD_TYPE STREQUAL "Debug")
include(CodeCoverage)
append_coverage_compiler_flags_to_target(test)
append_coverage_compiler_flags_to_target(${LIB_NAME})
setup_target_for_coverage_lcov(
            NAME test_coverage
            EXECUTABLE test
            DEPENDENCIES test ${LIB_NAME}
            EXCLUDE "*settings*"
    )
endif()
