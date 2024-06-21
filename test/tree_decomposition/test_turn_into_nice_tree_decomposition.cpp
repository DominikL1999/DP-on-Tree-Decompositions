#include "tree_decomposition.h"
#include "util.h"

using std::cout;
using std::endl;

bool test_easy() {
    std::vector<std::string>test_names{"cycle", "house", "k4_plus_2_appendages", "k4_plus_3_appendages", "k4_plus_4_appendages", "sigma_graph"};
    for (const std::string& test_name : test_names) {
        cout << "TEST_NAME: " << test_name << endl;
        UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/unit-test-instances/" + test_name + ".gr.csv");
        TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/unit-test-instances/" + test_name + ".td.csv", graph);
        Node_Id root = td.rootTree();

        td.turnIntoNiceTreeDecomposition();
        bool success = td.isNiceTreeDecomposition();

        if (!success) {
            return false;
        }
    }

    return true;
}

bool test_medium() {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/unit-test-instances/house.gr.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/unit-test-instances/house_with_unneccesary_join.td.csv", graph);
    
    Node_Id root = td.rootTree();
    td.turnIntoNiceTreeDecomposition();
    return td.isNiceTreeDecomposition();
}

bool test_hard() {
    bool success = true;
    for (int i = 1; i < 1; i) {
        UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/Treewidth-PACE-2017-Instances/ex00" + std::to_string(i) + ".gr.csv");
        TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/Treewidth-PACE-2017-Instances/ex00" + std::to_string(i) + ".gr.csv", graph);
        Node_Id root = td.rootTree();

        td.turnIntoNiceTreeDecomposition();
        if (!td.isNiceTreeDecomposition()) {
            return false;
        }
    }

    return true;
}

int test_turn_into_nice_tree_decomposition(int argc, char** argv) {
    bool success = true;

    success &= test_hard();
    success &= test_medium();
    success &= test_easy();

    return !success;
}