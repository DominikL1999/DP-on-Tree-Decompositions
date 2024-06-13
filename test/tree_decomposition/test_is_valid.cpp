#include "undirected_graph.h"
#include "tree_decomposition.h"
#include "util.h"

bool test_is_valid_valid_td() {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/parsing-test-instances/cycle.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/parsing-test-instances/cycle_td.csv", graph);

    return td.isValid();
}

bool test_is_valid_vertex_missing() {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/parsing-test-instances/cycle.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/parsing-test-instances/cycle_td_vertex_missing.csv", graph);

    return !td.isValid();
}

bool test_is_valid_edge_missing() {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/parsing-test-instances/cycle.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/parsing-test-instances/cycle_td_edge_missing.csv", graph);

    return !td.isValid();
}

bool test_is_valid_subtree_disconnected() {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/parsing-test-instances/cycle.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/parsing-test-instances/cycle_td_subtree_disconnected.csv", graph);

    return !td.isValid();
}

int test_is_valid(int argc, char** argv) {
    bool success = true;
    success &= test_is_valid_valid_td();
    success &= test_is_valid_vertex_missing();
    success &= test_is_valid_edge_missing();
    success &= test_is_valid_subtree_disconnected();

    return !success;
}