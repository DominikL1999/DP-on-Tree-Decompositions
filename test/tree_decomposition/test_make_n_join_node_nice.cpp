#include "tree_decomposition.h"
#include "util.h"

#include <cassert>

using std::cout;
using std::endl;

int test_make_n_join_node_nice(int argc, char** argv) {
    bool success = true;
    for (int appendages = 2; appendages <= 4; appendages++) {
        UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/unit-test-instances/k4_plus_" + std::to_string(appendages) + "_appendages.gr.csv");
        TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/unit-test-instances/k4_plus_" + std::to_string(appendages) + "_appendages.td.csv", graph);

        success &= returnAndOutputOnFailure(true, td.isValid());

        td.rootTree(td.nameToId("N1"));
        success &= returnAndOutputOnFailure(true, td.isValid());
        success &= returnAndOutputOnFailure(true, td.isRooted());

        td.makeNJoinNodeNice(td.nameToId("N1"));

        success &= returnAndOutputOnFailure(true, td.isNiceTreeDecomposition());
        if (!success)
            break;
    }

    return !success;
}
