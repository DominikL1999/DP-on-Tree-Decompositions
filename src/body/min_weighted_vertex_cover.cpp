#include "min_weighted_vertex_cover.h"

#include <cassert>
#include <numeric>

using std::cout;
using std::endl;

Solution MinWeightedVertexCover::solve()
{
    assert(td.isRooted());
    assert(td.isNiceTreeDecomposition());

    int counter = 0;

    td.doSomethingPostOrder([this, &counter](const Node_Id t_id) {
        counter++;
        assert(M.find(t_id) == M.end());
        M.insert({t_id, {}});

        // update M here
        const auto t = td.getNode(t_id);
        
        if (t.children.empty()) { // is a leaf node
            assert(t.bag.size() == 1);
            Vertex_Id v = *t.bag.begin();

            M[t_id][{}] = {{}, 0};
            M[t_id][{v}] = {{v}, graph.getWeight(v)};
            assertMContainsTheWholeBag(t_id);
        }
        else if (t.children.size() == 1) {
            Node_Id t_prime_id = *t.children.begin();
            const auto t_prime = td.getNode(t_prime_id);
            assert(M.find(t_prime_id) != M.end()); // todo: remove
            const auto prime_table = M.at(t_prime_id);
            size_t first_prime_table_size = M.at(t_prime_id).size();
            assert(prime_table.size() == M.at(t_prime_id).size());

            if (t.bag.size() > t_prime.bag.size()) { // node is an introduce node
                assert(t.bag.size() - t_prime.bag.size() == 1);
                // get extra vertex
                auto diff = setDifferrence(t.bag, t_prime.bag);
                assert(diff.size() == 1);
                Vertex_Id v_id = *diff.begin();
                assert(prime_table.size() == M.at(t_prime_id).size());

                for (const auto pair : M.at(t_prime_id)) {
                    const Vertex_Cover& U_prime = pair.first;
                    Vertex_Cover_Weight weight = pair.second.total_weight;
                    M[t_id][setUnion(U_prime, {v_id})] = addToSolution(M.at(t_prime_id).at(U_prime), v_id);
                    Vertex_Cover forbidden_neighbours = setDifferrence(t_prime.bag, U_prime);
                    bool is_vertex_cover = !std::any_of(forbidden_neighbours.begin(), forbidden_neighbours.end(), [this, v_id](Vertex_Id v2_id) {
                        return graph.areNeighbours(v2_id, v_id);
                    });
                    if (is_vertex_cover) {
                        M[t_id][U_prime] = M.at(t_prime_id).at(U_prime);
                    }
                }
                assertMContainsTheWholeBag(t_id);
                assert(prime_table.size() == first_prime_table_size); // If this goes off, then the local prime_table was changed somehow
                assert(prime_table.size() == M.at(t_prime_id).size());
            }
            else { // node is a forget node
                assert(t_prime.bag.size() - t.bag.size() == 1);
                // get extra vertex
                auto diff = setDifferrence(t_prime.bag, t.bag);
                assert(diff.size() == 1);
                Vertex_Id v_id = *diff.begin();
                assert(prime_table.size() == M.at(t_prime_id).size());
                for (const auto pair : M.at(t_prime_id)) {
                    
                    const Vertex_Cover& U_prime = pair.first;
                    assert(M.at(t_prime_id).find(U_prime) != M.at(t_prime_id).end());
                    Vertex_Cover small_U_prime = setDifferrence(U_prime, {v_id});
                    auto it = M.at(t_prime_id).find(small_U_prime);
                    const auto& sol_U_prime = M.at(t_prime_id).at(U_prime);
                    if (it == M.at(t_prime_id).end()) {
                        M[t_id][small_U_prime] = sol_U_prime;
                    }
                    else {
                        const auto& sol_small_U_prime = M.at(t_prime_id).at(small_U_prime);
                        M[t_id][small_U_prime] = std::min(sol_U_prime, sol_small_U_prime);
                    }
                    // M[t_id][U_prime] = std::min(M.at(t_prime_id).at(U_prime), M.at(t_prime_id).at(small_U_prime));
                    assert(prime_table.size() == first_prime_table_size); // If this goes off, then the local prime_table was changed somehow
                    assert(prime_table.size() == M.at(t_prime_id).size());
                }
                assertMContainsTheWholeBag(t_id);
                assert(prime_table.size() == first_prime_table_size); // If this goes off, then the local prime_table was changed somehow
                assert(prime_table.size() == M.at(t_prime_id).size());
            }

            assert(prime_table.size() == M.at(t_prime_id).size());

            // remove all entries for the child to reclaim memory space.
            M.erase(t_prime_id);
        }
        else { // is a join node
            assert(t.children.size() == 2);
            auto it = t.children.begin();
            Node_Id t1_id = *it;
            it++;
            Node_Id t2_id = *it;
            const auto t1 = td.getNode(t1_id);
            const auto t2 = td.getNode(t2_id);

            // 1. Intersect the U_prime for the left and the right subtree.
            auto intersected_U_prime = intersect(t1_id, t2_id);

            // 2. Go over the intersection and update M[t_id]
            for (const Vertex_Cover U_prime : intersected_U_prime) {
                Vertex_Cover_Weight weight1 = M.at(t1_id).at(U_prime).total_weight;
                Vertex_Cover_Weight weight2 = M.at(t2_id).at(U_prime).total_weight;
                auto extra_weight = std::accumulate(U_prime.begin(), U_prime.end(), 0, [this](Vertex_Cover_Weight weight, Vertex_Id v_id){return weight + graph.getWeight(v_id);});
                M[t_id][U_prime] = {U_prime, weight1 + weight2 - extra_weight};
            }
            assertMContainsTheWholeBag(t_id);

            // remove all entries for both children to reclaim memory space.
            M.erase(t1_id);
            M.erase(t2_id);
        }

        assert(M.at(t_id).find(t.bag) != M.at(t_id).end());
    });

    Node_Id root_id = td.getRoot();
    auto min_solution = *std::min_element(M.at(root_id).begin(), M.at(root_id).end(), [](const std::pair<Vertex_Cover, Solution>& pair1, const std::pair<Vertex_Cover, Solution>& pair2){
        return pair1.second.total_weight < pair2.second.total_weight;
    });

    return min_solution.second;
}

void MinWeightedVertexCover::assertMContainsTheWholeBag(Node_Id n_id) const {
    const Node& n = td.getNode(n_id);
    assert(M.at(n_id).find(n.bag) != M.at(n_id).end());
}

void MinWeightedVertexCover::printM() const {
    cout << "====== printM 1 ======" << endl;
    for (const auto& pair : M) {
        std::string node_name = td.getNode(pair.first).name;
        cout << node_name << "(" << pair.first << "): ";
        for (const std::pair<Vertex_Cover, Solution>& inner_pair : pair.second) {
            cout << "{";
            for (Vertex_Id v_id : inner_pair.first) {
                cout << graph.idToName(v_id) << "(" << v_id << ") ";
            };
            cout << "} -> " << inner_pair.second.total_weight << " | ";
        }
        cout << endl;
    }
    cout << "====== printM 2 ======" << endl;
}

std::unordered_set<Vertex_Cover> MinWeightedVertexCover::intersect(Node_Id n1_id, Node_Id n2_id) const {
    std::unordered_set<Vertex_Cover> covers1;
    std::unordered_set<Vertex_Cover> covers2;
    // todo: fix this to actually return the intersections of U_prime and U_prime_prime
    for (const std::pair<Vertex_Cover, Solution>& bla : M.at(n1_id)) {
        covers1.insert(bla.first);
    }
    for (const std::pair<Vertex_Cover, Solution>& bla : M.at(n2_id)) {
        covers2.insert(bla.first);
    }
    return setIntersect(covers1, covers2);
}

Solution MinWeightedVertexCover::addToSolution(const Solution &sol, Vertex_Id v_id) const
{
    assert(!sol.past_vertex_cover.contains(v_id));
    return {setUnion(sol.past_vertex_cover, {v_id}), sol.total_weight + graph.getWeight(v_id)};
}

std::ostream &operator<<(std::ostream &os, const Solution &sol) {
    return os << std::pair<Vertex_Cover, Vertex_Cover_Weight>(sol.past_vertex_cover, sol.total_weight);
}

bool operator<(const Solution &sol1, const Solution &sol2)
{
    return sol1.total_weight < sol2.total_weight;
}
