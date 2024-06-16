#include "tree_decomposition.h"
#include "util.h"

#include <cassert>

using std::cout;
using std::endl;

bool test_bridge_difference_order(std::string root, const std::vector<std::string>& names) {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/unit-test-instances/cycle.gr.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/unit-test-instances/cycle.td.csv", graph);
    bool success = true;

    success &= returnAndOutputOnFailure(false, td.getNodes().empty());
    success &= returnAndOutputOnFailure(true, td.isValid());
    success &= returnAndOutputOnFailure(false, td.isRooted());
    success &= returnAndOutputOnFailure(false, td.isNiceTreeDecomposition());
    
    td.rootTree(td.nameToId(root));
    success &= returnAndOutputOnFailure(true, td.isValid());
    success &= returnAndOutputOnFailure(true, td.isRooted());
    success &= returnAndOutputOnFailure(false, td.isNiceTreeDecomposition());

    for (size_t name_pos = 0; name_pos < names.size() - 1; name_pos++) {
        td.bridgeDifference(td.nameToId(names[name_pos]));
        success &= returnAndOutputOnFailure(false, td.isNiceTreeDecomposition());
    }
    
    td.bridgeDifference(td.nameToId(names[names.size() - 1]));
    success &= returnAndOutputOnFailure(true, td.isNiceTreeDecomposition());

    return success;
}

bool test_bridge_difference_2() {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/unit-test-instances/sigma_graph.gr.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/unit-test-instances/sigma_graph.td.csv", graph);

    td.rootTree(td.nameToId("N1"));
    td.bridgeDifference(td.nameToId("N1"));

    bool success = true;
    
    success &= returnAndOutputOnFailure(true, td.isValid());
    success &= returnAndOutputOnFailure(true, td.isNiceTreeDecomposition());

    return success;
}

int test_bridge_difference(int argc, char** argv) {
    bool success = true;

    success &= test_bridge_difference_2();

    success &= test_bridge_difference_order("N1", {"N1","N2","N3"});
    success &= test_bridge_difference_order("N1", {"N1","N3","N2"});
    success &= test_bridge_difference_order("N1", {"N2","N1","N3"});
    success &= test_bridge_difference_order("N1", {"N2","N3","N1"});
    success &= test_bridge_difference_order("N1", {"N3","N1","N2"});
    success &= test_bridge_difference_order("N1", {"N3","N2","N1"});

    success &= test_bridge_difference_order("N4", {"N4","N2","N3"});
    success &= test_bridge_difference_order("N4", {"N4","N3","N2"});
    success &= test_bridge_difference_order("N4", {"N2","N4","N3"});
    success &= test_bridge_difference_order("N4", {"N2","N3","N4"});
    success &= test_bridge_difference_order("N4", {"N3","N4","N2"});
    success &= test_bridge_difference_order("N4", {"N3","N2","N4"});

    return !success;
}
