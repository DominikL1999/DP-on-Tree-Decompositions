#include "undirected_graph.h"
#include "tree_decomposition.h"
#include "min_weighted_vertex_cover.h"
#include "util.h"

#include <iostream>

using std::cout;
using std::endl;

void printUsage(const std::string& errorMessage)
{
    cout << "Error: " << errorMessage << endl;
    printf("Usage:\n"
       "./main <graph-infile> <td-infile>\n"
       "\n"
       "Description:\n"
       "    Runs the MINIMUM_WEIGHT_VERTEX_COVER solver on the given graph infile using the given tree decomposition.\n"
      );
}

bool parseArguments(int argc, char* argv[], std::string& input_path, std::string& td_input_path) {
    if (argc < 3) {
        printUsage("At least 2 arguments expected.");
        return false;
    }
    input_path = argv[1];
    td_input_path = argv[2];

    return true;
}

void outputSolution(const UndirectedGraph& graph, const Solution& solution) {
    std::vector<Vertex_Id>vertex_cover_vector{solution.past_vertex_cover.begin(), solution.past_vertex_cover.end()};

    cout << "Minimum Vertex Cover: {";
    for (size_t i = 0; i < vertex_cover_vector.size(); i++) {
        Vertex_Id v_id = vertex_cover_vector[i];
        cout << graph.idToName(v_id) << (i == vertex_cover_vector.size() - 1 ? "" : " ");
    }
    cout << "}" << endl;
    cout << "Weight: " << solution.total_weight << endl;
}

int main(int argc, char* argv[]) {
    std::string input_path;
    std::string td_input_path;

    if (!parseArguments(argc, argv, input_path, td_input_path))
        return 1;

    UndirectedGraph graph = UndirectedGraph::parseUnsafe(input_path);
    TreeDecomposition td = TreeDecomposition::parseUnsafe(td_input_path, graph);

    td.rootTree();
    cout << "Turn into nice tree decomposition..." << endl;
    td.turnIntoNiceTreeDecomposition();

    MinWeightedVertexCover solver{graph, td};
    cout << "Tree decomposition has treewidth " << td.getTreewidth() << "." << endl;
    cout << "Starting to solve..." << endl;
    auto solution = solver.solve();
    outputSolution(graph, solution);
}