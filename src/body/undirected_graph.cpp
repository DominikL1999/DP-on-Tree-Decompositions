#include "undirected_graph.h"
#include "util.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

using std::string;

/*
Given the path to a CSV file, parses that file as an `UndirectedGraph` object.

I. a,b     ... Undirected, unlabelled edge
II. a,,20   ... Label vertex (i.e. set weight)
*/
UndirectedGraph UndirectedGraph::parseUnsafe(const string& input_path) {
    std::ifstream input(input_path);
    if (!input)
        throw std::invalid_argument::exception();

    UndirectedGraph graph;

    string line;
    while (getline(input, line)) {
        if (line == "")
            continue;
        
        std::vector<string> comma_parts = stringSplit(line, ',');
        const std::string& v1_name = comma_parts[0];
        const std::string& v2_name = comma_parts[1];
        if (comma_parts.size() == 2) { // Case I
            Vertex_Id v1_id = graph.addVertex(v1_name);
            Vertex_Id v2_id = graph.addVertex(v2_name);
            graph.addEdge(v1_id, v2_id);
        }
        else if (comma_parts.size() == 3) { // Case II
            const std::string& label = comma_parts[2];
            Vertex_Id v_id = graph.addVertex(v1_name);
            graph.setWeight(v_id, std::stod(label)); // immediately parse as number (double)
        }
        else {
            throw std::invalid_argument::exception();
        }
    }

    return graph;
}

std::string UndirectedGraph::idToName(Vertex_Id v_id) const {
    return vertex_id_to_name.at(v_id);
}

Vertex_Id UndirectedGraph::nameToId(const std::string &name) const
{
    if (!vertex_name_to_id.contains(name)) {
        throw std::invalid_argument("Does not know " + name);
    }
    return vertex_name_to_id.at(name);
}

size_t UndirectedGraph::numberOfNodes() const {
    return vertices.size();
}

size_t UndirectedGraph::numberOfEdges() const {
    return edges.size();
}

const std::vector<Vertex_Id>& UndirectedGraph::getVertices() const {
    return vertices;
}

const std::vector<Edge>& UndirectedGraph::getEdges() const {
    return edges;
}

const Vertex_Weight UndirectedGraph::getWeight(Vertex_Id v_id) const {
    return vertex_id_to_weight[v_id];
}

const std::vector<Vertex_Id>& UndirectedGraph::getNeighbours(Vertex_Id v_id) const {
    return adjacencies.at(v_id);
}

bool UndirectedGraph::areNeighbours(Vertex_Id v_id1, Vertex_Id v_id2) const {
    return contains(adjacencies[v_id1], v_id2);
}

Vertex_Id UndirectedGraph::addVertex(const string &v_name) {
    Vertex_Id new_id;
    if (!vertex_name_to_id.contains(v_name)) {
        new_id = next_free_id++;

        vertices.push_back(new_id);
        adjacencies.push_back({});

        vertex_id_to_name.push_back(v_name);
        vertex_id_to_weight.push_back({});
        vertex_name_to_id.insert({v_name, new_id});
    }
    else {
        new_id = vertex_name_to_id.at(v_name);
    }

    return new_id;
}

void UndirectedGraph::setWeight(Vertex_Id v_id, Vertex_Weight label) {
    vertex_id_to_weight[v_id] = label;
}

void UndirectedGraph::addEdge(Vertex_Id v1_id, Vertex_Id v2_id) {
    adjacencies[v1_id].push_back(v2_id);
    adjacencies[v2_id].push_back(v1_id);
    edges.push_back({std::min(v1_id, v2_id), std::max(v1_id, v2_id)});
}

std::ostream& operator<<(std::ostream& stream, const UndirectedGraph& graph) {
    stream << graph.numberOfNodes() << " vertices, " << graph.numberOfEdges() << " edges." << std::endl;
    stream << "vertex labels:" << std::endl;
    for (Vertex_Id v_id = 0; v_id < graph.numberOfNodes(); v_id++) {
        stream << "    " << graph.vertex_id_to_name.at(v_id) << ": " << graph.vertex_id_to_weight.at(v_id);
    }

    stream << std::endl << "edges:" << std::endl;
    for (Vertex_Id v_id1 = 0; v_id1 < graph.numberOfNodes(); v_id1++) {
        for (const Vertex_Id& v_id2 : graph.adjacencies[v_id1]) {
            if (v_id1 > v_id2)
                continue;
            stream << "(" << graph.vertex_id_to_name.at(v_id1) << "," << graph.vertex_id_to_name.at(v_id2) << ")" << std::endl;
        }
    }

    return stream;
}
