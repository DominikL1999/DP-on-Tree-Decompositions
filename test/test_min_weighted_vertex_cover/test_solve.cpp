#include "undirected_graph.h"
#include "tree_decomposition.h"
#include "min_weighted_vertex_cover.h"
#include "util.h"

#include <cassert>

using std::cout;
using std::endl;

void solve_unit_test_instance(const std::string& graph_file, const std::string& td_file, int expected_min_vertex_cover_weight) {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/unit-test-instances/" + graph_file);
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/unit-test-instances/" + td_file, graph);

    cout << "graph_file: " << graph_file << endl;
    Node_Id root_id = td.rootTree();
    td.turnIntoNiceTreeDecomposition(root_id);
    MinWeightedVertexCover solver{graph, td};
    cout << "td: " << td << endl;

    Solution solution = solver.solve();

    assert(doubleEqual(solution.past_vertex_cover_weight, expected_min_vertex_cover_weight));
}

bool solve_unit_test_instances() {
    solve_unit_test_instance("cycle.gr.csv", "cycle.td.csv", 90);
    solve_unit_test_instance("house.gr.csv", "house.td.csv", 70);
    solve_unit_test_instance("k4_plus_2_appendages.gr.csv", "k4_plus_2_appendages.td.csv", 3);
    solve_unit_test_instance("k4_plus_3_appendages.gr.csv", "k4_plus_3_appendages.td.csv", 3);
    solve_unit_test_instance("k4_plus_4_appendages.gr.csv", "k4_plus_4_appendages.td.csv", 4);
    solve_unit_test_instance("sigma_graph.gr.csv", "sigma_graph.td.csv", 9);

    return true;
}

int test_solve(int argc, char** argv) {
    assert(solve_unit_test_instances());

    return 0;
}