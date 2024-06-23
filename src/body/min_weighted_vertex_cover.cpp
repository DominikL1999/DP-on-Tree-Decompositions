#include "min_weighted_vertex_cover.h"

#include <numeric>

using std::cout;
using std::endl;

/*
In this implementation, when updating the table value at a node n: The author has made the concsious decision to not compute *all* possible 2^bagsize vertex covers, but to derive only the valid vertex covers based on the valid vertex covers of the nodes child(ren) and save the weight of those covers in a map.
*/
Solution MinWeightedVertexCover::solve() {
    td.doSomethingPostOrder([this](const Node_Id t_id) {
        M.insert({t_id, {}});

        // update M here
        const auto t = td.getNode(t_id);
        
        if (t.children.empty()) { // is a leaf node
            Vertex_Id v = *t.bag.begin();

            M[t_id][{}] = {{}, 0};
            M[t_id][{v}] = {{v}, graph.getWeight(v)};
        }
        else if (t.children.size() == 1) {
            Node_Id t_prime_id = *t.children.begin();
            const auto& t_prime = td.getNode(t_prime_id);

            if (t.bag.size() > t_prime.bag.size()) { // node is an introduce node
                // get extra vertex
                auto diff = setDifferrence(t.bag, t_prime.bag);
                Vertex_Id v_id = *diff.begin();

                for (const auto pair : M.at(t_prime_id)) {
                    const Vertex_Cover& U_prime = pair.first;
                    M[t_id][setUnion(U_prime, {v_id})] = addToSolution(M.at(t_prime_id).at(U_prime), v_id);
                    Vertex_Cover forbidden_neighbours = setDifferrence(t_prime.bag, U_prime);
                    bool is_vertex_cover = !std::any_of(forbidden_neighbours.begin(), forbidden_neighbours.end(), [this, v_id](Vertex_Id v2_id) {
                        return graph.areNeighbours(v2_id, v_id);
                    });
                    if (is_vertex_cover) {
                        M[t_id][U_prime] = M.at(t_prime_id).at(U_prime);
                    }
                }
            }
            else { // node is a forget node
                // get extra vertex
                auto diff = setDifferrence(t_prime.bag, t.bag);
                Vertex_Id v_id = *diff.begin();
                for (const auto pair : M.at(t_prime_id)) {
                    
                    const Vertex_Cover& U_prime = pair.first;
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
                }
            }

            // remove all entries for the child to reclaim memory space.
            M.erase(t_prime_id);
        }
        else { // is a join node
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

            // remove all entries for both children to reclaim memory space.
            M.erase(t1_id);
            M.erase(t2_id);
        }
    });

    // Return minimum weight solution in root
    Node_Id root_id = td.getRoot();
    auto min_solution = *std::min_element(M.at(root_id).begin(), M.at(root_id).end(), [](const std::pair<Vertex_Cover, Solution>& pair1, const std::pair<Vertex_Cover, Solution>& pair2){
        return pair1.second.total_weight < pair2.second.total_weight;
    });

    return min_solution.second;
}

std::unordered_set<Vertex_Cover> MinWeightedVertexCover::intersect(Node_Id n1_id, Node_Id n2_id) const {
    std::unordered_set<Vertex_Cover> covers1;
    std::unordered_set<Vertex_Cover> covers2;
    for (const std::pair<Vertex_Cover, Solution>& pair : M.at(n1_id)) {
        covers1.insert(pair.first);
    }
    for (const std::pair<Vertex_Cover, Solution>& pair : M.at(n2_id)) {
        covers2.insert(pair.first);
    }
    return setIntersect(covers1, covers2);
}

Solution MinWeightedVertexCover::addToSolution(const Solution &sol, Vertex_Id v_id) const {
    return {setUnion(sol.past_vertex_cover, {v_id}), sol.total_weight + graph.getWeight(v_id)};
}

std::ostream &operator<<(std::ostream &os, const Solution &sol) {
    return os << std::pair<Vertex_Cover, Vertex_Cover_Weight>(sol.past_vertex_cover, sol.total_weight);
}

bool operator<(const Solution &sol1, const Solution &sol2) {
    return sol1.total_weight < sol2.total_weight;
}
