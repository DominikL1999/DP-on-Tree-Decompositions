#pragma once

#include "util.h"
#include "undirected_graph.h"
#include "tree_decomposition.h"

class MinWeightedVertexCover {

public:
    using Vertex_Cover = std::unordered_set<Vertex_Id>;
    using Vertex_Cover_Weight = Vertex_Weight;
    using Solution = std::pair<Vertex_Cover_Weight, Vertex_Cover>;
    using Table = std::unordered_map<Vertex_Cover, Vertex_Weight>;

    MinWeightedVertexCover(const UndirectedGraph& graph_, const TreeDecomposition& td_) : graph(graph_), td(td_) {}

    MinWeightedVertexCover::Solution solve();

private:
    const UndirectedGraph& graph;
    const TreeDecomposition& td;

    std::unordered_map<Node_Id, Table>M;
};