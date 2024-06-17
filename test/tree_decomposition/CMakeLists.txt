CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

set (TEST_FILES
    test_bridge_difference.cpp;
    test_is_valid.cpp;
    test_make_n_join_node_nice.cpp;
    test_parse_unsafe.cpp;
    test_root_tree.cpp;
    test_turn_into_nice_tree_decomposition.cpp)

string(REPLACE "${CMAKE_SOURCE_DIR}/" "" TestSuiteName "${CMAKE_CURRENT_SOURCE_DIR}")
string(REPLACE "/" "_" TestSuiteName ${TestSuiteName})

create_test_sourcelist (Tests ${TestSuiteName}.cpp ${TEST_FILES})

# add executable
add_executable(${TestSuiteName} ${Tests})
target_link_libraries(${TestSuiteName} PUBLIC DP-ON-TREE-DECOMPOSITIONS_LIB)
target_include_directories(${TestSuiteName} PUBLIC ${HEADER_DIR})

# Add ADD_TEST for each test
foreach (test ${TEST_FILES})
    get_filename_component (TName ${test} NAME_WE)
    add_test (NAME ${TName} COMMAND ${TestSuiteName} ${TName} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endforeach ()
