#include "tree_decomposition.h"
#include "undirected_graph.h"
#include "util.h"

#include <fstream>
#include <cassert>

using std::string;

/*
Given the path to a CSV file, parses that file as an `TreeDecomposition` object.

Cases:
I. N1,N2        ... Edge between nodes
II. N1,,a;b;c   ... Set bag for node
*/
TreeDecomposition TreeDecomposition::parseUnsafe(const std::string &input_path, const UndirectedGraph& graph_)
{
    std::ifstream input(input_path);

    TreeDecomposition td{graph_};

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
            auto bag_contents = map<string, Vertex_Id>(bag_contents_names, [&graph_](const std::string& v_name){return graph_.nameToId(v_name);});
            td.setBag(n_id, bag_contents);
            td.treewidth = std::max(td.treewidth, bag_contents_names.size() - 1);
        }
        else {
            throw std::invalid_argument::exception();
        }
    }

    return td;
}

bool TreeDecomposition::isValid(const UndirectedGraph& graph) {
    // TODO
    return false;
}

bool TreeDecomposition::areNeighbours(Node_Id n_id1, Node_Id n_id2) const {
    return contains(adjacencies[n_id1], n_id2);
}

std::string TreeDecomposition::idToName(Node_Id n_id) const
{
    return node_id_to_name.at(n_id);
}

Node_Id TreeDecomposition::nameToId(const std::string& name) const
{
    return node_name_to_id.at(name);
}

const std::vector<Vertex_Id> &TreeDecomposition::getBag(Node_Id n_id) const {
    return node_id_to_bag_contents[n_id];
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
}

void TreeDecomposition::setBag(Node_Id n_id, const std::vector<Vertex_Id>& bag_content) {
    node_id_to_bag_contents[n_id] = std::vector<Vertex_Id>{bag_content.begin(), bag_content.end()};
}
