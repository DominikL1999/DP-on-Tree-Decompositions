#pragma once

#include <iostream>
#include <cstdio>
#include <vector>
#include <unordered_map>

using Vertex_Id = std::size_t;
using Vertex_Label = double;
using AdjacencyLists = std::vector<std::vector<Vertex_Id>>;

/*
Class for undirected graphs (with number labels for vertices) as required for the MINIMUM WEIGHTED VERTEX COVER problem.
*/
class UndirectedGraph {
    std::vector<Vertex_Id> vertices;
    std::vector<Vertex_Label> vertex_labels;
    AdjacencyLists adjacencies;

    // The names of the vertices (usually just numbers). This is *not* the label of the vertex, because the labels represent a vertices' weight.
    std::vector<std::string> vertex_id_to_name;
    std::unordered_map<std::string, Vertex_Id> vertex_name_to_id;

    size_t next_free_id = 0;
    size_t n_edges = 0;

public:
    static UndirectedGraph parseUnsafe(const std::string& input_path);

    size_t number_of_vertices() const;

    size_t number_of_edges() const;

    friend
    std::ostream& operator<<(std::ostream& stream, const UndirectedGraph& graph);

private:

    // adds new vertex with given name if this vertex has not been added before
    void addVertex(const std::string& name);

    void labelVertex(const std::string& vertex_name, Vertex_Label label);

    // adds new edge. If the edge_type is different than what was previously seen, instead returns false
    void addEdge(const std::string& v1, const std::string& v2);
};
