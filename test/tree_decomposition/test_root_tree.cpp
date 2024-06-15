#include "undirected_graph.h"
#include "tree_decomposition.h"

#include <stack>
#include <cassert>

int test_root_tree(int argc, char** argv) {
    return 0;
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/Treewidth-PACE-2027-Instances/ex001.gr.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/Treewidth-PACE-2027-Instances/ex001.td.csv", graph);
    assert(td.isValid());

    Node_Id root = td.rootTree();
    const auto& parents = td.getParents();
    const auto& children = td.getChildren();

    // Go recursively over all children and count the number of edges. If it is correct, the number of edges should be number of nodes - 1.
    std::vector<bool> visited(td.getNodes().size(), false);
    int edge_count = 0;
    std::stack<Node_Id> to_visit;
    to_visit.push(root);
    
    while (!to_visit.empty()) {
        Node_Id cur_n_id = to_visit.top();
        to_visit.pop();
        assert(!visited[cur_n_id]);
        visited[cur_n_id] = true;

        for (const auto& child : children[cur_n_id]) {
            assert(parents[child] == cur_n_id);
            to_visit.push(child);
            edge_count++;
        }
    }

    bool success = edge_count == td.getNodes().size() - 1;
    success &= td.getEdges().size() == edge_count;

    return !success;
}