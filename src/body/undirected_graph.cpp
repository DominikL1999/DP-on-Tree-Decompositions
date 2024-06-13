#include "undirected_graph.h"
#include "util.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

using std::string;

/*
Given the path to a CSV file, parses that file as an `UndirectedGraph` object.

1. a,b     ... Undirected, unlabelled edge
2. a,,20   ... Label vertex (i.e. set weight)
*/
UndirectedGraph UndirectedGraph::parseUnsafe(const string& input_path) {
    std::ifstream input(input_path);

    UndirectedGraph graph;

    string line;
    while (getline(input, line)) {
        std::vector<string> comma_parts = stringSplit(line, ',');
        const std::string& v1_name = comma_parts[0];
        const std::string& v2_name = comma_parts[1];
        if (comma_parts.size() == 2) {
            graph.addVertex(v1_name);
            graph.addVertex(v2_name);
            graph.addEdge(v1_name, v2_name);
        }
        else if (comma_parts.size() == 3) {
            const std::string& label = comma_parts[2];
            graph.addVertex(v1_name);
            graph.labelVertex(v1_name, std::stod(label)); // immediately parse as number (double)
        }
        else {
            throw std::invalid_argument::exception();
        }
    }

    return graph;
}

size_t UndirectedGraph::number_of_vertices() const
{
    return vertices.size();
}

size_t UndirectedGraph::number_of_edges() const
{
    return n_edges;
}

void UndirectedGraph::addVertex(const string &name) {
    if (!vertex_name_to_id.contains(name)) {
        Vertex_Id new_id = next_free_id++;

        vertices.push_back(new_id);
        vertex_labels.resize(vertex_labels.size() + 1);
        adjacencies.resize(adjacencies.size() + 1);
        vertex_id_to_name.push_back(name);
        vertex_name_to_id.insert({name, new_id});
    }
}

void UndirectedGraph::labelVertex(const string &vertex_name, Vertex_Label label) {
    Vertex_Id v_id = vertex_name_to_id.at(vertex_name);
    vertex_labels[v_id] = label;
}

void UndirectedGraph::addEdge(const string& v1, const string& v2) {
    Vertex_Id v1_id = vertex_name_to_id.at(v1);
    Vertex_Id v2_id = vertex_name_to_id.at(v2);
    adjacencies[v1_id].push_back(v2_id);
    adjacencies[v2_id].push_back(v1_id);
    n_edges++;
}

std::ostream& operator<<(std::ostream& stream, const UndirectedGraph& graph) {
    stream << graph.number_of_vertices() << " vertices, " << graph.number_of_edges() << " edges." << std::endl;
    stream << "vertex labels:" << std::endl;
    for (Vertex_Id v_id = 0; v_id < graph.number_of_vertices(); v_id++) {
        stream << "    " << graph.vertex_id_to_name.at(v_id) << ": " << graph.vertex_labels.at(v_id);
    }

    stream << std::endl << "edges:" << std::endl;
    for (Vertex_Id v_id1 = 0; v_id1 < graph.number_of_vertices(); v_id1++) {
        for (const Vertex_Id& v_id2 : graph.adjacencies[v_id1]) {
            if (v_id1 > v_id2)
                continue;
            stream << "(" << graph.vertex_id_to_name.at(v_id1) << "," << graph.vertex_id_to_name.at(v_id2) << ")" << std::endl;
        }
    }

    return stream;
}
