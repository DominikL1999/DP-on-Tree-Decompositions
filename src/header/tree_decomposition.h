#pragma once

#include "undirected_graph.h"

#include <iostream>
#include <unordered_set>
#include <functional>
#include <optional>

using Node_Id = size_t;
using TreeDecompositionAdjacencies = std::vector<std::unordered_set<Node_Id>>;
using Bag = std::unordered_set<Vertex_Id>;
using TD_Edge = std::pair<Node_Id, Node_Id>;

template<>
struct std::hash<TD_Edge>
{
    size_t operator()(const TD_Edge& td_edge) const {
        size_t seed = 0;
        seed ^= std::hash<int>{}(td_edge.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<int>{}(td_edge.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

class TreeDecomposition {
    std::vector<Node_Id> nodes;
    std::unordered_set<TD_Edge> edges;
    TreeDecompositionAdjacencies adjacencies;

    std::vector<std::string> node_id_to_name;
    std::vector<Bag> node_id_to_bag_contents;
    std::unordered_map<std::string, Node_Id> node_name_to_id;

    const UndirectedGraph* graph_ptr;

    size_t next_free_id = 0;
    size_t treewidth = 0;

    // To root tree:
    std::optional<Node_Id> root={};
    std::vector<std::optional<Node_Id>> parents;
    std::vector<std::unordered_set<Node_Id>> children;

    // To make it a nice tree decomposition:
    size_t new_nodes_counter=0;

public:

    static TreeDecomposition parseUnsafe(const std::string& input_path, const UndirectedGraph& graph);

    // Returns true if it is a valid tree decomposition
    bool isValid() const;

    const std::vector<Node_Id>& getNodes() const;

    const std::unordered_set<TD_Edge>& getEdges() const;

    bool areNeighbours(Node_Id n_id1, Node_Id n_id2) const;

    const std::unordered_set<Node_Id>& getNeighbours(Node_Id n_id) const;

    std::string idToName(Node_Id n_id) const;

    Node_Id nameToId(const std::string& name) const;

    const Bag& getBag(Node_Id n_id) const;

    const std::vector<Bag>& getBags() const;

    size_t getTreewidth() const;

    //// To root tree ////

    bool isRooted() const;

    const Node_Id& getRoot() const;

    // Returns the children and parents of each node in the tree decomposition and picks the root to be one of the nodes with the largest bag. Returns the root node.
    Node_Id rootTree();

    // Returns the children and parents of each node in the tree decomposition if the root were `designated_root`.
    void rootTree(Node_Id designated_root);

    const std::vector<std::optional<Node_Id>>& getParents() const;

    const std::vector<std::unordered_set<Node_Id>>& getChildren() const;

    const std::unordered_set<Node_Id>& getChildrenOf(Node_Id n_id) const;

    //// To make it a nice tree decomposition ////

    bool isNiceTreeDecomposition() const;

    // Turns this tree decomposition into a nice tree decomposition.
    void turnIntoNiceTreeDecomposition();

    // Given the id of a node with exactly one child, fills the space between it and its child such that every node in between is either an introduce node or a forget node.
    void bridgeDifference(Node_Id parent_id);

    // Given the id of a node with more than one child, fills the space between it and its children such that every node in between is either a join node or an introduce node or a forget node.
    void makeNJoinNodeNice(Node_Id parent_id);

    std::string printBag(const Bag& bag) const;

    std::string printChildren(Node_Id n_id) const;

    void doSomethingPreOrder(std::function<void(Node_Id)>f) const;
    
    void doSomethingPreOrder(std::function<void(Node_Id)>f, Node_Id n_id) const;

    void doWhilePreOrder(std::function<void(Node_Id)>f, std::function<bool()>pred) const;

    void doWhilePreOrder(std::function<void(Node_Id)>f, std::function<bool()>pred, Node_Id n_id) const;

    friend
    std::ostream& operator<<(std::ostream& stream, const TreeDecomposition& td);

private:
    TreeDecomposition(const UndirectedGraph& graph) : graph_ptr(&graph) {}

    Node_Id addNode();

    Node_Id addNode(const std::string& n_name);
    
    void addEdge(Node_Id n1_id, Node_Id n2_id);

    void removeEdge(Node_Id n1_id, Node_Id n2_id);

    void setBag(Node_Id n_id, const Bag& bag_content); // copy bag

    void setBag(Node_Id n_id, Bag&& bag_content); // move existing bag
};
