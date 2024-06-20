CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

# List all subdirectories containing tests here
set(test_directories
    tree_decomposition;
    test_min_weighted_vertex_cover;
    undirected_graph;
    util
)

foreach (dir ${test_directories})
    add_subdirectory(${dir})
endforeach()
