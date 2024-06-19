#include "min_weighted_vertex_cover.h"

#include <cassert>
#include <numeric>

using std::cout;
using std::endl;

Solution MinWeightedVertexCover::solve()
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
            Vertex_Id v = *t.bag.begin();
            M[t_id][{}] = {{}, 0};
            M[t_id][{v}] = {{v}, graph.getWeight(v)};
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
                    const Vertex_Cover& U_prime_cover_rest = pair.second.vertex_cover;
                    Vertex_Cover_Weight weight = pair.second.vertex_cover_weight;
                    M[t_id][setUnion(U_prime, {v_id})] = addToSolution(M[t_prime_id][U_prime], v_id);
                    if (isVertexCoverWithout(M[t_id][U_prime], v_id)) {
                        M[t_id][U_prime] = M[t_prime_id][U_prime];
                    }
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
                    const Vertex_Cover& U_prime_cover_rest = pair.second.vertex_cover;
                    Vertex_Cover_Weight weight = pair.second.vertex_cover_weight;
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

            // 1. Intersect the U_prime for the left and the right subtree.
            auto intersected_U_prime = intersect(t1_id, t2_id);
            // std::unordered_set<Vertex_Cover> intersected_U_prime = intersect(t1_id, t2_id);

            // 2. Go over the intersection and update M[t_id]
            for (const Vertex_Cover& U_prime : intersected_U_prime) {
                Vertex_Cover_Weight weight1 = M[t1_id][U_prime].vertex_cover_weight;
                Vertex_Cover_Weight weight2 = M[t2_id][U_prime].vertex_cover_weight;
                auto extra_weight = std::accumulate(U_prime.begin(), U_prime.end(), 0, [this](Vertex_Cover_Weight weight, Vertex_Id v_id){return weight + graph.getWeight(v_id);});
                M[t_id][U_prime] = {U_prime, weight1 + weight2 - extra_weight};
            }
            // remove all entries for both children to reclaim memory space.
            // M.erase(t1_id);
            // M.erase(t2_id);
        }
    });

    assert(root_happened);

    Node_Id root_id = td.getRoot();
    auto bla = *std::min_element(M[root_id].begin(), M[root_id].end(), [](const std::pair<Vertex_Cover, Solution>& pair1, const std::pair<Vertex_Cover, Solution>& pair2){
        return pair1.second.vertex_cover_weight < pair2.second.vertex_cover_weight;
    });

    return bla.second;
}

void MinWeightedVertexCover::printM() const {
    for (const auto& pair : M) {
        std::string node_name = td.getNode(pair.first).name;
        cout << node_name << ": ";
        for (const std::pair<Vertex_Cover, Solution>& inner_pair : pair.second) {
            cout << "{";
            for (Vertex_Id v_id : inner_pair.first) {
                cout << graph.idToName(v_id) << " ";
            };
            cout << "} -> " << inner_pair.second.vertex_cover_weight << " | ";
        }
        cout << endl;
    }
}

bool MinWeightedVertexCover::isVertexCoverWithout(const Solution &m_t_u_prime, Vertex_Id v_id) const {
    // Go over all vertices in the partial solution and if any neighbour v_id, return false;
    const Vertex_Cover& potential_vertex_cover = m_t_u_prime.vertex_cover;
    return std::all_of(potential_vertex_cover.begin(), potential_vertex_cover.end(), [this, v_id](Vertex_Id cover_vertex_id){
        return !graph.areNeighbours(cover_vertex_id, v_id);
    });
}

std::unordered_set<Vertex_Cover> MinWeightedVertexCover::intersect(Node_Id n1_id, Node_Id n2_id) const {
    std::unordered_set<Vertex_Cover> covers1;
    std::unordered_set<Vertex_Cover> covers2;
    for (const std::pair<Vertex_Cover, Solution>& bla : M.at(n1_id)) {
        covers1.insert(bla.second.vertex_cover);
    }
    for (const std::pair<Vertex_Cover, Solution>& bla : M.at(n2_id)) {
        covers2.insert(bla.second.vertex_cover);
    }
    return setIntersect(covers1, covers2);
}

Solution MinWeightedVertexCover::addToSolution(const Solution &sol, Vertex_Id v_id) const
{
    assert(!sol.vertex_cover.contains(v_id));
    return {setUnion(sol.vertex_cover, {v_id}), sol.vertex_cover_weight + graph.getWeight(v_id)};
}

std::ostream &operator<<(std::ostream &os, const Solution &sol) {
    return os << std::pair<Vertex_Cover, Vertex_Cover_Weight>(sol.vertex_cover, sol.vertex_cover_weight);
}

bool operator<(const Solution &sol1, const Solution &sol2)
{
    return sol1.vertex_cover_weight < sol2.vertex_cover_weight;
}
