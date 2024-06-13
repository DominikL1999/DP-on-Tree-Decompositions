#include <iostream>

#include "undirected_graph.h"

using std::cout;
using std::endl;

void printUsage(const std::string& errorMessage)
{
    cout << "Error: " << errorMessage << endl;
    printf("Usage:\n"
       "./main <infile>\n"
       "\n"
       "Description:\n"
       "    Runs the MINIMUM_WEIGHT_VERTEX_COVER solver on the given input file.\n"
      );
}

bool parseArguments(int argc, char* argv[], std::string& input_path) {
    if (argc < 2) {
        printUsage("At least 1 argument expected.");
        return false;
    }
    input_path = argv[1];

    return true;
}

int main(int argc, char* argv[]) {
    std::string input_path;

    if (!parseArguments(argc, argv, input_path))
        return 1;

    UndirectedGraph graph = UndirectedGraph::parseUnsafe(input_path);

    cout << "graph: " << graph << endl;
}