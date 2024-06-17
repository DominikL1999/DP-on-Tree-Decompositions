#include "undirected_graph.h"
#include "tree_decomposition.h"
#include "util.h"

#include <cassert>
#include <filesystem>

using std::cout;
using std::endl;

bool test_is_valid_valid_td() {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/unit-test-instances/cycle.gr.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/unit-test-instances/cycle.td.csv", graph);

    cout << "td: " << td << endl;

    return td.isValid();
}

bool test_is_valid_vertex_missing() {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/unit-test-instances/cycle.gr.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/unit-test-instances/cycle_vertex_missing.td.csv", graph);

    return !td.isValid();
}

bool test_is_valid_edge_missing() {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/unit-test-instances/cycle.gr.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/unit-test-instances/cycle_edge_missing.td.csv", graph);

    return !td.isValid();
}

bool test_is_valid_subtree_disconnected() {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/unit-test-instances/cycle.gr.csv");
    TreeDecomposition td = TreeDecomposition::parseUnsafe("test-instances/unit-test-instances/cycle_subtree_disconnected.td.csv", graph);

    return !td.isValid();
}

bool test_is_valid_large_graphs() {
    const std::filesystem::path large_instances_path{"test-instances/Treewidth-PACE-2017-Instances"};
    std::vector<std::pair<std::string,std::string>> graph_td_files_pairs;
    for (auto const& dir_entry : std::filesystem::directory_iterator(large_instances_path)) {
        if (!endsWith(dir_entry.path(), ".gr.csv")) {
            continue;
        }

        const std::string filename = stripToFilename(dir_entry.path());
        if (endsWith(filename, ".gr.csv")) {
            graph_td_files_pairs.push_back({filename, filename.substr(0, filename.size() - 7) + ".td.csv"});
        }
        const std::string filename_no_extension = filename.substr(0, filename.size() - 7);

        const std::string graph_path = "test-instances/Treewidth-PACE-2017-Instances/" + filename_no_extension + ".gr.csv";
        const std::string td_path = "test-instances/Treewidth-PACE-2017-Instances/" + filename_no_extension + ".td.csv";

        UndirectedGraph graph = UndirectedGraph::parseUnsafe(graph_path);
        TreeDecomposition td = TreeDecomposition::parseUnsafe(td_path, graph);
        
        bool success = td.isValid();
        if (!success)
            return false;
    }
    return true;
}

int test_is_valid(int argc, char** argv) {
    bool success = true;
    
    success &= test_is_valid_valid_td();
    success &= test_is_valid_vertex_missing();
    success &= test_is_valid_edge_missing();
    success &= test_is_valid_subtree_disconnected();
    // success &= test_is_valid_large_graphs(); // this takes a long time (3 seconds total, but still.)

    return !success;
}