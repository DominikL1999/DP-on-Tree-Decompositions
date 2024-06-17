#include "undirected_graph.h"
#include "tree_decomposition.h"

#include <stack>
#include <cassert>

int test_root_tree(int argc, char** argv) {
    return 0;
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/Treewidth-PACE-2027-Instances/ex001.gr.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/Treewidth-PACE-2027-Instances/ex001.td.csv", graph);
    bool success = true;
    
    success &= td.isValid();

    Node_Id root = td.rootTree();
    
    success &= td.isValid();
    success &= td.isRooted();
}