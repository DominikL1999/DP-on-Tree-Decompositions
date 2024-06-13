#include <iostream>

#include "undirected_graph.h"
#include "tree_decomposition.h"
#include "util.h"

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

int main(int argc, char* argv[]) {
    std::string input_path;
    std::string td_input_path;

    if (!parseArguments(argc, argv, input_path, td_input_path))
        return 1;

    UndirectedGraph graph = UndirectedGraph::parseUnsafe(input_path);
    cout << "graph: " << graph << endl;
    TreeDecomposition td = TreeDecomposition::parseUnsafe(td_input_path, graph);
    cout << "tree decomposition: " << td << endl;
}