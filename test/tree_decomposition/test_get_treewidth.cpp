#include "undirected_graph.h"
#include "tree_decomposition.h"
#include "util.h"

#include <cassert>

int test_get_treewidth(int argc, char** argv) {
    std::vector<std::tuple<std::string, std::string, size_t>>tests{
        {"cycle.gr.csv", "cycle.td.csv", 2},
        {"house.gr.csv", "house.td.csv", 2},
        {"house.gr.csv", "house_with_unneccesary_join.td.csv", 2},
        {"k4_plus_2_appendages.gr.csv", "k4_plus_2_appendages.td.csv", 3},
        {"k4_plus_4_appendages.gr.csv", "k4_plus_4_appendages.td.csv", 3}
    };
    for (const auto& test : tests) {
        std::string graph_file = "test-instances/unit-test-instances/" + std::get<0>(test);
        std::string td_file = "test-instances/unit-test-instances/" + std::get<1>(test);

        UndirectedGraph graph = UndirectedGraph::parseUnsafe(graph_file);
        TreeDecomposition td = TreeDecomposition::parseUnsafe(td_file, graph);

        assert(returnAndOutputOnFailure(std::get<2>(test), td.getTreewidth()));
    }

    return 0;
}