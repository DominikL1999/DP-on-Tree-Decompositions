CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

# List all subdirectories containing tests here
set(test_directories
    util;
    tree_decomposition;
    undirected_graph)

foreach (dir ${test_directories})
    add_subdirectory(${dir})
endforeach()
