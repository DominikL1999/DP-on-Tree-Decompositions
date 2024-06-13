#pragma once

#include <iostream>
#include <cstdio>
#include <vector>
#include <unordered_map>

using Vertex_Id = std::size_t;
using Vertex_Label = double;
using UndirectedGraphAdjacencies = std::vector<std::vector<Vertex_Id>>;

/*
Class for undirected graphs (with number labels for vertices) as required for the MINIMUM WEIGHTED VERTEX COVER problem.
*/
class UndirectedGraph {
    std::vector<Vertex_Id> vertices;
    UndirectedGraphAdjacencies adjacencies;

    // The names of the vertices (usually just numbers). This is *not* the label of the vertex, because the labels represent a vertices' weight.
    std::vector<std::string> vertex_id_to_name;
    std::vector<Vertex_Label> vertex_id_to_label;
    std::unordered_map<std::string, Vertex_Id> vertex_name_to_id;

    size_t next_free_id = 0;
    size_t n_edges = 0;

public:
    static UndirectedGraph parseUnsafe(const std::string& input_path);

    std::string idToName(Vertex_Id v_id) const;

    Vertex_Id nameToId(const std::string& name) const;

    size_t numberOfNodes() const;

    size_t numberOfEdges() const;

    const std::vector<Vertex_Id>& getVertices() const;

    const std::vector<Vertex_Id>& getNeighbours(Vertex_Id v_id) const;

    bool areNeighbours(Vertex_Id v_id1, Vertex_Id v_id2) const;

    friend
    std::ostream& operator<<(std::ostream& stream, const UndirectedGraph& graph);

private:

    // TODO: Have this function return Vertex_Id and rework labelVertex and addEdge to take ids instead of names.
    // adds new vertex with given name if this vertex has not been added before
    Vertex_Id addVertex(const std::string& name);

    void labelVertex(Vertex_Id v_id, Vertex_Label label);

    // adds new edge. If the edge_type is different than what was previously seen, instead returns false
    void addEdge(Vertex_Id v_id1, Vertex_Id v_id2);
};
