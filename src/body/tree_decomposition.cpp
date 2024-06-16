#include "tree_decomposition.h"
#include "undirected_graph.h"
#include "util.h"

#include <fstream>
#include <cassert>
#include <stack>

using std::string;

/*
Given the path to a CSV file, parses that file as an `TreeDecomposition` object.

Cases:
I. N1,N2        ... Edge between nodes
II. N1,,a;b;c   ... Set bag for node
*/
TreeDecomposition TreeDecomposition::parseUnsafe(const std::string &input_path, const UndirectedGraph& graph)
{
    std::ifstream input(input_path);
    if (!input)
        throw std::invalid_argument::exception();

    TreeDecomposition td{graph};

    string line;
    while (getline(input, line)) {
        if (line == "")
            continue;

        std::vector<string> comma_parts = stringSplit(line, ',');
        std::string& n1_name = comma_parts[0];
        std::string& n2_name = comma_parts[1];
        
        if (comma_parts.size() == 2) { // Case I
            Node_Id n1_id = td.addNode(n1_name);
            Node_Id n2_id = td.addNode(n2_name);
            td.addEdge(n1_id, n2_id);
        }
        else if (comma_parts.size() == 3) { // Case II
            std::vector<string> bag_contents_names = stringSplit(comma_parts[2], ';');
            Node_Id n_id = td.addNode(n1_name);
            Bag bag;
            for (const string& v_name : bag_contents_names) {
                bag.insert(graph.nameToId(v_name));
            }
            td.setBag(n_id, std::move(bag));
            td.treewidth = std::max(td.treewidth, bag_contents_names.size() - 1);
        }
        else {
            throw std::invalid_argument::exception();
        }
    }

    assert(!td.nodes.empty());

    return td;
}

/*
A "nice" tree decomposition is a tree decomposition where each node falls into exactly one of four categories:
    I   Leaf Node       ... A node with no children and any number of vertices in its bag
    II  Introduce Node  ... A node whose bag contains exactly one additional vertex when compared to its (single) child's bag.
    III Forget Node     ... A node whose (single) child's bag contains exactly one additional vertex when compared to its own bag.
    IV  Join Node       ... A node with two children with all equal bags.

How to get a nice tree decomposition:
- Between any two (connected) nodes, "subdivide" their difference into multiple nodes.
- Let C be the set of children of a node n with bag B. If |C| >= 2, create a chain of nodes each with bag B the size of |C| - 2 and append it to n. Add n and all created nodes to "root_nodes". Assert(|root_nodes| == |C| - 1). The last node added should be added twice.
*/
void TreeDecomposition::turnIntoNiceTreeDecomposition() {

}

void TreeDecomposition::makeNJoinNodeNice(Node_Id n_id) {
    // std::vector<Node_Id> new_nodes;
    // Node_Id last_node = n_id;
    // TODO
}

bool TreeDecomposition::isValid() const {
    // Property 1: Every vertex appears in some bag
    const std::vector<Vertex_Id> vertices = graph_ptr->getVertices();
    bool success = std::all_of(vertices.begin(), vertices.end(), [this](const Vertex_Id& v_id){
        return std::any_of(nodes.begin(), nodes.end(), [this, v_id](const Node_Id& n_id){
            return contains(getBag(n_id), v_id);
        });
    });
    if (!success) {
        return false;
    }

    // Property 2: Every edge appears in some bag
    const std::vector<Edge>& edges = graph_ptr->getEdges();
    success = std::all_of(edges.begin(), edges.end(), [this](const Edge& edge){
        return std::any_of(getBags().begin(), getBags().end(), [&edge](const Bag& bag){
            return contains(bag, edge.first) && contains(bag, edge.second);
        });
    });
    if (!success) {
        return false;
    }

    // Property 3: For every vertex v, the subtree containing vertex v in its bags is connected.
    success = std::all_of(vertices.begin(), vertices.end(), [this](const Vertex_Id v_id){
        std::vector<Node_Id> visited;
        std::stack<Node_Id> to_visit;
        auto it = std::find_if(getNodes().begin(), getNodes().end(), [this,v_id](const Node_Id& n_id){return contains(getBag(n_id), v_id);});
        assert(it != getNodes().end());
        to_visit.push(*it);
        while (!to_visit.empty()) {
            Node_Id cur_n_id = to_visit.top();
            to_visit.pop();
            visited.push_back(cur_n_id);
            const std::unordered_set<Node_Id>& neighbours = getNeighbours(cur_n_id);
            for (const Node_Id& n_id : neighbours) {
                if (contains(visited, n_id) || (!contains(getBag(n_id), v_id)))
                    continue;
                else {
                    to_visit.push(n_id);
                }
            }
        }

        auto filtered = filter<Node_Id>(nodes, [this, v_id](Node_Id n_id){return contains(getBag(n_id), v_id);});
        return setEqual(visited, filter<Node_Id>(nodes, [this, v_id](Node_Id n_id){return contains(getBag(n_id), v_id);}));
    });
    if (!success) {
        return false;
    }

    return true;
}

const std::vector<Node_Id>& TreeDecomposition::getNodes() const {
    return nodes;
}

const std::unordered_set<TD_Edge>& TreeDecomposition::getEdges() const {
    return edges;
}

bool TreeDecomposition::areNeighbours(Node_Id n_id1, Node_Id n_id2) const {
    return contains(adjacencies[n_id1], n_id2);
}

const std::unordered_set<Node_Id>& TreeDecomposition::getNeighbours(Node_Id n_id) const {
    return adjacencies.at(n_id);
}

std::string TreeDecomposition::idToName(Node_Id n_id) const
{
    return node_id_to_name.at(n_id);
}

Node_Id TreeDecomposition::nameToId(const std::string& name) const
{
    return node_name_to_id.at(name);
}

const Bag& TreeDecomposition::getBag(Node_Id n_id) const {
    return node_id_to_bag_contents[n_id];
}

const std::vector<Bag>& TreeDecomposition::getBags() const {
    return node_id_to_bag_contents;
}

size_t TreeDecomposition::getTreewidth() const {
    return treewidth;
}

bool TreeDecomposition::isRooted() const {
    return root.has_value();
}

const Node_Id& TreeDecomposition::getRoot() const {
    assert(isRooted());
    return root.value();
}

Node_Id TreeDecomposition::rootTree() {
    assert(!nodes.empty());
    Node_Id node_with_largest_bag = *std::max_element(nodes.begin(), nodes.end(), [this](Node_Id n1_id, Node_Id n2_id) {
        return getBag(n1_id).size() <= getBag(n2_id).size();
    });

    rootTree(node_with_largest_bag);

    return node_with_largest_bag;
}

void TreeDecomposition::rootTree(Node_Id designated_root) {
    parents.clear();
    children.clear();
    parents.resize(nodes.size());
    children.resize(nodes.size());

    std::vector<bool> visited(nodes.size(), false);

    std::stack<Node_Id> to_visit;
    to_visit.push(designated_root);

    while (!to_visit.empty()) {
        Node_Id cur_n_id = to_visit.top();
        to_visit.pop();
        visited[cur_n_id] = true;
        const std::unordered_set<Node_Id>& neighbours = getNeighbours(cur_n_id);
        
        for (const Node_Id& neighbour : neighbours) {
            if (visited[neighbour])
                continue; // has been visited already
            else {
                parents[neighbour] = cur_n_id;
                children[cur_n_id].insert(neighbour);
                to_visit.push(neighbour);
            }
        }
    }
}

const std::vector<std::optional<Node_Id>>& TreeDecomposition::getParents() const {
    return parents;
}

const std::vector<std::unordered_set<Node_Id>>& TreeDecomposition::getChildren() const {
    return children;
}

Node_Id TreeDecomposition::addNode(const std::string &n_name) {
    Node_Id new_id;
    if (!node_name_to_id.contains(n_name)) {
        new_id = next_free_id++;
        nodes.push_back(new_id);
        adjacencies.push_back({});

        node_id_to_name.push_back(n_name);
        node_id_to_bag_contents.push_back({});
        node_name_to_id.insert({n_name, new_id});
    }
    else {
        new_id = node_name_to_id.at(n_name);
    }

    assert(!nodes.empty());

    return new_id;
}

void TreeDecomposition::addEdge(Node_Id n1_id, Node_Id n2_id) {
    adjacencies[n1_id].insert(n2_id);
    adjacencies[n2_id].insert(n1_id);
    edges.insert({std::min(n1_id, n2_id), std::max(n1_id, n2_id)});
}

void TreeDecomposition::removeEdge(Node_Id n1_id, Node_Id n2_id) {
    assert(areNeighbours(n1_id, n2_id));

    // Remove edge in adjacencies
    adjacencies[n1_id].erase(n2_id);
    adjacencies[n2_id].erase(n1_id);

    // Remove edge in edges
    edges.erase({std::min(n1_id, n2_id), std::max(n1_id, n2_id)});

    // If tree is rooted: Remove edge in parents and children.
    if (isRooted()) {
        parents[n2_id] = {};
        children[n1_id].erase(n2_id);
    }
}

void TreeDecomposition::setBag(Node_Id n_id, Bag&& bag_content) {
    std::swap(node_id_to_bag_contents[n_id], bag_content);
}

std::ostream &operator<<(std::ostream &stream, const TreeDecomposition &td)
{
    stream << "Tree Decomposition with " << td.getNodes().size() << " nodes. Those nodes are:" << std::endl;
    for (const Node_Id n_id : td.getNodes()) {
        const Bag& bag = td.getBag(n_id);
        stream << td.idToName(n_id) << ": ";
        for (const Vertex_Id& v_id : bag) {
            stream << td.graph_ptr->idToName(v_id) << " ";
        }
        stream << std::endl;
    }
    stream << "Edges:" << std::endl;
    for (const TD_Edge& edge : td.getEdges()) {
        const std::pair<std::string, std::string> edge_view{td.idToName(edge.first), td.idToName(edge.second)};
        stream << edge_view << " ";
    }
    stream << std::endl;
    return stream;
}
