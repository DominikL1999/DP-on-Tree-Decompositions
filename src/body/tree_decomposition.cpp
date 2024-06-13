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
            td.setBag(n_id, map<string, Vertex_Id>(bag_contents_names, [&graph](const std::string& v_name){return graph.nameToId(v_name);}));
            td.treewidth = std::max(td.treewidth, bag_contents_names.size() - 1);
        }
        else {
            throw std::invalid_argument::exception();
        }
    }

    return td;
}

bool TreeDecomposition::isValid() const {
    // Property 1: Every vertex appears in some bag
    const UndirectedGraph& graph = *graph_ptr;
    const std::vector<Vertex_Id> vertices = graph.getVertices();
    bool success = std::all_of(vertices.begin(), vertices.end(), [this](const Vertex_Id& v_id){
        return std::any_of(nodes.begin(), nodes.end(), [this, v_id](const Node_Id& n_id){
            return contains(getBag(n_id), v_id);
        });
    });
    if (!success)
        return false;

    // Property 2: Every edge appears in some bag
    const std::vector<Edge>& edges = graph.getEdges();
    success = std::all_of(edges.begin(), edges.end(), [this](const Edge& edge){
        return std::any_of(getBags().begin(), getBags().end(), [&edge](const Bag& bag){
            return contains(bag, edge.first) && contains(bag, edge.second);
        });
    });
    if (!success)
        return false;

    // Property 3: For every vertex v, the subtree containing vertex v in its bags is connected.
    success = std::all_of(vertices.begin(), vertices.end(), [this](const Vertex_Id v_id){
        std::cout << "v_id: " << v_id << std::endl;
        std::vector<Node_Id> visited;
        std::stack<Node_Id> to_visit;
        auto it = std::find_if(getNodes().begin(), getNodes().end(), [this,v_id](const Node_Id& n_id){return contains(getBag(n_id), v_id);});
        assert(it != getNodes().end());
        to_visit.push(*it);
        while (!to_visit.empty()) {
            Node_Id cur = to_visit.top();
            to_visit.pop();
            visited.push_back(cur);
            const std::vector<Node_Id>& neighbours = getNeighbours(cur);
            for (const Node_Id& n_id : neighbours) {
                if (contains(visited, n_id) || (!contains(getBag(n_id), v_id)))
                    continue;
                else {
                    to_visit.push(n_id);
                }
            }
        }

        auto filtered = filter<Node_Id>(nodes, [this, v_id](Node_Id n_id){return contains(getBag(n_id), v_id);});
        std::cout << "visited: " << visited << std::endl;
        std::cout << "filtered: " << filtered << std::endl;
        return setEqual(visited, filter<Node_Id>(nodes, [this, v_id](Node_Id n_id){return contains(getBag(n_id), v_id);}));
    });
    if (!success)
        return false;

    return true;
}

const std::vector<Node_Id>& TreeDecomposition::getNodes() const {
    return nodes;
}

const std::vector<TD_Edge>& TreeDecomposition::getEdges() const {
    return edges;
}

bool TreeDecomposition::areNeighbours(Node_Id n_id1, Node_Id n_id2) const {
    return contains(adjacencies[n_id1], n_id2);
}

const std::vector<Node_Id>& TreeDecomposition::getNeighbours(Node_Id n_id) const {
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

    return new_id;
}

void TreeDecomposition::addEdge(Node_Id n1_id, Node_Id n2_id) {
    adjacencies[n1_id].push_back(n2_id);
    adjacencies[n2_id].push_back(n1_id);
    edges.push_back({std::min(n1_id, n2_id), std::max(n1_id, n2_id)});
}

void TreeDecomposition::setBag(Node_Id n_id, const std::vector<Vertex_Id>& bag_content) {
    node_id_to_bag_contents[n_id] = std::vector<Vertex_Id>{bag_content.begin(), bag_content.end()};
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
