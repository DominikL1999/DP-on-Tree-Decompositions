#include "tree_decomposition.h"
#include "util.h"

#include <cassert>

using std::cout;
using std::endl;

int test_make_n_join_node_nice(int argc, char** argv) {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/unit-test-instances/house.gr.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/unit-test-instances/house.td.csv", graph);

    bool success = true;
    success &= returnAndOutputOnFailure(true, td.isValid());

    td.rootTree(td.nameToId("N1"));
    success &= returnAndOutputOnFailure(true, td.isValid());
    success &= returnAndOutputOnFailure(true, td.isRooted());

    td.makeNJoinNodeNice(td.nameToId("N1"));
    td.bridgeDifference(td.nameToId("N2"));
    td.bridgeDifference(td.nameToId("N3"));

    success &= returnAndOutputOnFailure(true, td.isNiceTreeDecomposition());

    return !success;
}
