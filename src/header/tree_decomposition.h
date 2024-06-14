#pragma once

#include "undirected_graph.h"

#include <iostream>
#include <optional>

using Node_Id = size_t;
using TreeDecompositionAdjacencies = std::vector<std::vector<Node_Id>>;
using Bag = std::vector<Vertex_Id>;
using TD_Edge = std::pair<Node_Id, Node_Id>;

class TreeDecomposition {
    std::vector<Node_Id> nodes;
    std::vector<TD_Edge> edges;
    TreeDecompositionAdjacencies adjacencies;

    std::vector<std::string> node_id_to_name;
    std::vector<Bag> node_id_to_bag_contents;
    std::unordered_map<std::string, Node_Id> node_name_to_id;

    const UndirectedGraph* graph_ptr;

    size_t next_free_id = 0;
    size_t treewidth = 0;

public:

    static TreeDecomposition parseUnsafe(const std::string& input_path, const UndirectedGraph& graph);

    // Returns true if it is a valid tree decomposition
    bool isValid() const;

    const std::vector<Node_Id>& getNodes() const;

    const std::vector<TD_Edge>& getEdges() const;

    bool areNeighbours(Node_Id n_id1, Node_Id n_id2) const;

    const std::vector<Node_Id>& getNeighbours(Node_Id n_id) const;

    std::string idToName(Node_Id n_id) const;

    Node_Id nameToId(const std::string& name) const;

    const Bag& getBag(Node_Id n_id) const;

    const std::vector<Bag>& getBags() const;

    size_t getTreewidth() const;

    // Returns the children and parents of each node in the tree decomposition and picks the root to be one of the nodes with the largest bag. Returns the root node.
    Node_Id rootTree(std::vector<std::optional<Node_Id>>& parents, std::vector<std::vector<Node_Id>>& children) const;

    // Returns the children and parents of each node in the tree decomposition if the root were `designated_root`.
    void rootTree(std::vector<std::optional<Node_Id>>& parents, std::vector<std::vector<Node_Id>>& children, Node_Id designated_root) const;

    friend
    std::ostream& operator<<(std::ostream& stream, const TreeDecomposition& td);

private:
    TreeDecomposition(const UndirectedGraph& graph) : graph_ptr(&graph) {}

    Node_Id addNode(const std::string& n_name);
    
    void addEdge(Node_Id n1_id, Node_Id n2_id);

    void setBag(Node_Id n_id, const std::vector<Vertex_Id>& bag_content);
};
