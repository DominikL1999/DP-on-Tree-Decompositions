#pragma once

#include "undirected_graph.h"

#include <iostream>

using Node_Id = size_t;
using TreeDecompositionAdjacencies = std::vector<std::vector<Node_Id>>;

class TreeDecomposition {
    std::vector<Node_Id> nodes;
    TreeDecompositionAdjacencies adjacencies;

    std::vector<std::string> node_id_to_name;
    std::vector<std::vector<Vertex_Id>> node_id_to_bag_contents;
    std::unordered_map<std::string, Node_Id> node_name_to_id;

    const UndirectedGraph* graph;

    size_t next_free_id = 0;
    size_t treewidth = 0;

public:

    static TreeDecomposition parseUnsafe(const std::string& input_path, const UndirectedGraph& graph);

    // Returns true if it is a valid tree decomposition for the given undirected graph.
    bool isValid(const UndirectedGraph& graph);

    bool areNeighbours(Node_Id n_id1, Node_Id n_id2) const;

    std::string idToName(Node_Id n_id) const;

    Node_Id nameToId(const std::string& name) const;

    const std::vector<Vertex_Id>& getBag(Node_Id n_id) const;

    size_t getTreewidth() const;

    friend
    std::ostream& operator<<(std::ostream& stream, const TreeDecomposition& tree_decomposition);

private:
    TreeDecomposition(const UndirectedGraph& graph) : graph(&graph) {}

    Node_Id addNode(const std::string& n_name);
    
    void addEdge(Node_Id n1_id, Node_Id n2_id);

    void setBag(Node_Id n_id, const std::vector<Vertex_Id>& bag_content);
};
