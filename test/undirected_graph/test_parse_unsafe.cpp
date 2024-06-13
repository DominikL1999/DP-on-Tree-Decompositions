#include "undirected_graph.h"
#include "util.h"

bool test_parse_unsafe_1() {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/parsing-test-instances/k_5.csv");
    bool success = returnAndOutputOnFailure((size_t)5, graph.numberOfNodes());
    success &= returnAndOutputOnFailure((size_t)10, graph.numberOfEdges());
    for (Vertex_Id v_id1 = 0; v_id1 < graph.numberOfNodes() - 1; v_id1++) {
        const auto& neighbours = graph.getNeighbours(v_id1);
        for (Vertex_Id v_id2 = v_id1 + 1; v_id2 < graph.numberOfNodes(); v_id2++) {
            if (!graph.areNeighbours(v_id1, v_id2)) {
                success &= false;
                std::cout << "Two vertices should be neighbours, but aren't: (" << graph.idToName(v_id1) << "," << graph.idToName(v_id2) << ")" << std::endl;
                break;
            }
            if (!contains(neighbours, v_id2)) {
                success &= false;
                std::cout << "Two vertices should be neighbours, but aren't: (" << graph.idToName(v_id1) << "," << graph.idToName(v_id2) << ")" << std::endl;
                break;
            }
        }
    }

    std::vector<std::string>names{"a","b","c","d","e"};
    success &= setEqual(map<std::string,Vertex_Id>(names, [&graph](const std::string& name){return graph.nameToId(name);}), graph.getVertices());

    return success;
}

int test_parse_unsafe(int argc, char** argv) {
    bool success = test_parse_unsafe_1();
    
    return !success;
}