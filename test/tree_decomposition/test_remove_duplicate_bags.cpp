#include "tree_decomposition.h"
#include "util.h"

#include <cassert>

using std::cout;
using std::endl;

bool house_with_1_duplicate() {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/unit-test-instances/house.gr.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/unit-test-instances/house_with_1_duplicate.td.csv", graph);

    td.rootTree(td.nameToId("N1"));
    td.removeDuplicateNeighbours();

    assert(td.isValid());
    assert(setEqual(td.getAllNodeNames(), {"N1","N2","N3","N4"}));

    return true;
}

bool house_with_more_duplicates() {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/unit-test-instances/house.gr.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/unit-test-instances/house_with_more_duplicates.td.csv", graph);

    td.rootTree(td.nameToId("N1"));
    td.removeDuplicateNeighbours();

    assert(td.isValid());
    cout << "td.getAllNodeNames(): " << td.getAllNodeNames() << endl;
    assert(setEqual(td.getAllNodeNames(), {"N1","N2","N3","N4","N6"}));

    return true;
}

int test_remove_duplicate_bags(int argc, char** argv) {
    bool success = house_with_1_duplicate() && house_with_more_duplicates();

    return !success;
}