CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(DP-ON-TREE-DECOMPOSITIONS)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR src)
set(HEADER_DIR ${SRC_DIR}/header)
set(BODY_DIR ${SRC_DIR}/body)
set(TEST_DIR test)

set(HEADER_FILES
    ${HEADER_DIR}/undirected_graph.h;
    ${HEADER_DIR}/min-vertex-cover.h;
    ${HEADER_DIR}/tree_decomposition.h;
    ${HEADER_DIR}/util.h
)

set(BODY_FILES
    ${BODY_DIR}/undirected_graph.cpp;
    ${BODY_DIR}/min-vertex-cover.cpp;
    ${BODY_DIR}/tree_decomposition.cpp;
    ${BODY_DIR}/util.cpp
)

add_library(DP-ON-TREE-DECOMPOSITIONS_LIB STATIC ${HEADER_FILES} ${BODY_FILES})
target_link_libraries(DP-ON-TREE-DECOMPOSITIONS_LIB)
target_include_directories(DP-ON-TREE-DECOMPOSITIONS_LIB
    PUBLIC ${HEADER_DIR}
    PRIVATE ${BODY_DIR})

add_executable(main ${SRC_DIR}/main.cpp)
target_link_libraries(main PUBLIC DP-ON-TREE-DECOMPOSITIONS_LIB)
target_include_directories(main PUBLIC
    ${HEADER_DIR})

# Include CTest before running any tests
include(CTest)

# Add Tests
add_subdirectory(${TEST_DIR})