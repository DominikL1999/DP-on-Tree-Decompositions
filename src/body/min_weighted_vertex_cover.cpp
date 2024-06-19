#include "min_weighted_vertex_cover.h"

#include <cassert>
#include <numeric>

using std::cout;
using std::endl;

MinWeightedVertexCover::Solution MinWeightedVertexCover::solve()
{
    assert(td.isRooted());
    assert(td.isNiceTreeDecomposition());

    bool root_happened = false;

    td.doSomethingPostOrder([this, &root_happened](const Node_Id t_id) {
        assert((td.getRoot() == t_id) || (!root_happened));

        // update M here
        const auto& t = td.getNode(t_id);
        
        if (t.children.empty()) { // is a leaf node
            assert(t.bag.size() == 1);
            M[t_id][t.children] = graph.getWeight(*t.bag.begin());
        }
        else if (t.children.size() == 1) {
            Node_Id t_prime_id = *t.children.begin();
            const auto& t_prime = td.getNode(t_prime_id);
            if (t.bag.size() > t_prime.bag.size()) { // node is an introduce node
                assert(t.bag.size() - t_prime.bag.size() == 1);
                // get extra vertex
                auto diff = setDifferrence(t.bag, t_prime.bag);
                assert(diff.size() == 1);
                Vertex_Id v_id = *diff.begin();
                for (const auto& pair : M[t_prime_id]) {
                    const Vertex_Cover& U_prime = pair.first;
                    Vertex_Cover_Weight weight = pair.second;
                    // todo: only copy M[t_id][]
                    M[t_id][U_prime] = M[t_prime_id][U_prime];
                    M[t_id][setUnion(U_prime, {v_id})] = M[t_prime_id][U_prime] + graph.getWeight(v_id);
                }

                // remove all entries for the child to reclaim memory space.
                // M.erase(t_prime_id);
            }
            else { // node is a forget node
                assert(t_prime.bag.size() - t.bag.size() == 1);
                // get extra vertex
                auto diff = setDifferrence(t_prime.bag, t.bag);
                assert(diff.size() == 1);
                Vertex_Id v_id = *diff.begin();
                for (const auto& pair : M[t_prime_id]) {
                    const Vertex_Cover& U_prime = pair.first;
                    Vertex_Cover_Weight weight = pair.second;
                    M[t_id][U_prime] = std::min(M[t_prime_id][U_prime], M[t_prime_id][setDifferrence(U_prime, {v_id})]);
                }

                // remove all entries for the child to reclaim memory space.
                // M.erase(t_prime_id);
            }
        }
        else { // is a join node
            if (td.getRoot() == t_id) {
                root_happened = true;
            }
            assert(t.children.size() == 2);
            auto it = t.children.begin();
            Node_Id t1_id = *it;
            it++;
            Node_Id t2_id = *it;
            const auto& t1 = td.getNode(t1_id);
            const auto& t2 = td.getNode(t2_id);
            // todo: fix this. The set of valid U_prime and all U_prime_prime might not be the same.
            for (const auto& pair : M[t1_id]) {
                const Vertex_Cover& U_prime = pair.first;
                Vertex_Cover_Weight weight1 = pair.second;
                Vertex_Cover_Weight weight2 = M[t2_id][U_prime];
                auto extra_weight = std::accumulate(U_prime.begin(), U_prime.end(), 0, [this](Vertex_Cover_Weight weight, Vertex_Id v_id){return weight + graph.getWeight(v_id);});
                M[t_id][U_prime] = weight1 + weight2 - extra_weight;
            }
            // remove all entries for both children to reclaim memory space.
            // M.erase(t1_id);
            // M.erase(t2_id);
        }
    });

    assert(root_happened);

    Node_Id root_id = td.getRoot();
    auto bla = *std::min_element(M[root_id].begin(), M[root_id].end(), [](auto& pair1, auto& pair2){
        return pair1.second < pair2.second;
    });

    return bla;
}

void MinWeightedVertexCover::printM() const {
    for (const auto& pair : M) {
        std::string node_name = td.getNode(pair.first).name;
        cout << node_name << ": ";
        for (const auto& inner_pair : pair.second) {
            cout << "{";
            for (Vertex_Id v_id : inner_pair.first) {
                cout << graph.idToName(v_id) << " ";
            };
            cout << "} -> " << inner_pair.second << " | ";
        }
        cout << endl;
    }
}
