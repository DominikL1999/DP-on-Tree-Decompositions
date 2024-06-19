#pragma once

#include "util.h"
#include "undirected_graph.h"
#include "tree_decomposition.h"

using Vertex_Cover = std::unordered_set<Vertex_Id>;
using Vertex_Cover_Weight = Vertex_Weight;

struct Solution {
    Vertex_Cover vertex_cover;
    Vertex_Cover_Weight vertex_cover_weight;
};

std::ostream& operator<<(std::ostream& os, const Solution& sol);

bool operator<(const Solution& sol1, const Solution& sol2);

using Table = std::unordered_map<Vertex_Cover, Solution>;

class MinWeightedVertexCover {

public:

    MinWeightedVertexCover(const UndirectedGraph& graph_, const TreeDecomposition& td_) : graph(graph_), td(td_) {}

    Solution solve();

    void printM() const;

private:
    const UndirectedGraph& graph;
    const TreeDecomposition& td;

    std::unordered_map<Node_Id, Table>M;

    // Returns thue, if the partial solution is a vertex cover *without* vertex with id `v_id`.
    bool isVertexCoverWithout(const Solution& sol, Vertex_Id v_id) const;

    std::unordered_set<Vertex_Cover> intersect(Node_Id n1_id, Node_Id n2_id) const;

    Solution addToSolution(const Solution& sol, Vertex_Id v_id) const;
};