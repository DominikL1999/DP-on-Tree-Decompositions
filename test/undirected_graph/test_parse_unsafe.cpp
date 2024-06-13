#include "undirected_graph.h"
#include "util.h"

int test_parse_unsafe(int argc, char** argv) {
    UndirectedGraph graph = UndirectedGraph::parseUnsafe("test-instances/parsing-test-instances/k_5.csv");
    bool success1 = returnAndOutputOnFailure((size_t)5, graph.numberOfNodes());
    bool success2 = returnAndOutputOnFailure((size_t)10, graph.numberOfEdges());
    bool success3 = true;
    for (Vertex_Id v_id1 = 0; v_id1 < graph.numberOfNodes() - 1; v_id1++) {
        const auto& neighbours = graph.getNeighbours(v_id1);
        for (Vertex_Id v_id2 = v_id1 + 1; v_id2 < graph.numberOfNodes(); v_id2++) {
            if (!graph.areNeighbours(v_id1, v_id2)) {
                success3 = false;
                std::cout << "Two vertices should be neighbours, but aren't: (" << graph.idToName(v_id1) << "," << graph.idToName(v_id2) << ")" << std::endl;
                break;
            }
            if (!contains(neighbours, v_id2)) {
                success3 = false;
                std::cout << "Two vertices should be neighbours, but aren't: (" << graph.idToName(v_id1) << "," << graph.idToName(v_id2) << ")" << std::endl;
                break;
            }
        }
    }

    return !(success1 &&
        success2 &&
        success3);
}