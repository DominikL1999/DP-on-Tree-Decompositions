CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

# List the files containing tests here.
set (TEST_FILES
    test_undirected_graph_parse_unsafe.cpp)

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
