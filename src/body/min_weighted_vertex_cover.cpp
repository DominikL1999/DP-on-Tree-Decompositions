#include "min_weighted_vertex_cover.h"

#include <cassert>

MinWeightedVertexCover::Solution MinWeightedVertexCover::solve()
{
    assert(td.isRooted());
    assert(td.isNiceTreeDecomposition());

    td.doSomethingPostOrder([this](const Node_Id n_id) {
        // update M here
        const auto& node = td.getNode(n_id);
        
        if (node.children.empty()) { // is a leaf node
            Vertex_Weight sum = 0;
            for (const Vertex_Id& v_id : node.bag) {
                sum += graph.getWeight(v_id);
            }
            M[n_id][node.children] = sum;
        }
        else if (node.children.size() == 1) {
            const auto& child = td.getNode(*node.children.begin());
            if (node.bag.size() > child.bag.size()) { // node is an introduce node
                
            }
            else { // node is a forget node

            }
        }
        else { // is a join node
        }

        // remove all entries for the children to save memory.
    });

    return Solution();
}
