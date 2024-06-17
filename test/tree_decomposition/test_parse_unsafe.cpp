#include "undirected_graph.h"
#include "tree_decomposition.h"
#include "util.h"

bool test_parse_unsafe_1() {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/unit-test-instances/cycle.gr.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/unit-test-instances/cycle.td.csv", graph);

    bool success = returnAndOutputOnFailure((size_t)2, td.getTreewidth());

    Node_Id n1_id = td.nameToId("N1");
    Node_Id n2_id = td.nameToId("N2");
    Node_Id n3_id = td.nameToId("N3");
    Node_Id n4_id = td.nameToId("N4");

    // Check connections between bags
    success &= returnAndOutputOnFailure(true, td.areNeighbours(n1_id, n2_id));
    success &= returnAndOutputOnFailure(true, td.areNeighbours(n2_id, n3_id));
    success &= returnAndOutputOnFailure(true, td.areNeighbours(n3_id, n4_id));
    success &= returnAndOutputOnFailure(false, td.areNeighbours(n1_id, n3_id));
    success &= returnAndOutputOnFailure(false, td.areNeighbours(n1_id, n4_id));
    success &= returnAndOutputOnFailure(false, td.areNeighbours(n2_id, n4_id));

    // Check bag contents
    success &= returnAndOutputOnFailure({graph.nameToId("a"), graph.nameToId("b"), graph.nameToId("c")}, td.getNode(n1_id).bag);
    success &= returnAndOutputOnFailure({graph.nameToId("a"), graph.nameToId("c"), graph.nameToId("d")}, td.getNode(n2_id).bag);
    success &= returnAndOutputOnFailure({graph.nameToId("a"), graph.nameToId("d"), graph.nameToId("e")}, td.getNode(n3_id).bag);
    success &= returnAndOutputOnFailure({graph.nameToId("a"), graph.nameToId("e"), graph.nameToId("f")}, td.getNode(n4_id).bag);

    return success;
}

int test_parse_unsafe(int argc, char** argv) {
    bool success = test_parse_unsafe_1();

    return !success;
}