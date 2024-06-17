#include "tree_decomposition.h"
#include "undirected_graph.h"
#include "util.h"

#include <fstream>
#include <cassert>
#include <stack>

using std::cout; // todo: remove
using std::endl; // todo: remove
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
    if (!input) {
        throw std::invalid_argument::exception();
    }

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
            td.nodes[n_id].bag = bag;
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

bool TreeDecomposition::isNiceTreeDecomposition() const
{
    assert(isValid());
    assert(isRooted());
    
    bool is_valid = true;
    doWhilePreOrder([this, &is_valid](const Node_Id& n_id) {
        const auto& node = nodes.at(n_id);
        if (node.children.size() == 0) {
            return;
        }
        else if (node.children.size() == 1) {
            Node_Id child_id = *node.children.begin();
            const auto& child = nodes.at(child_id);
            size_t diff1 = setDifferrence(node.bag, child.bag).size();
            size_t diff2 = setDifferrence(child.bag, node.bag).size();
            if (!(diff1 == 0 && diff2 == 1 || diff1 == 1 && diff2 == 0)) {
                is_valid = false;
            }
            return;
        }

        for (const Node_Id& child_id : node.children) {
            const auto& child = nodes.at(child_id);
            if (child.bag != node.bag) {
                is_valid = false;
                return;
            }
        }
    }, [&is_valid](){return is_valid;});

    return is_valid;
}

void TreeDecomposition::turnIntoNiceTreeDecomposition() {
    assert(isRooted());
    turnIntoNiceTreeDecomposition(getRoot());
}

void TreeDecomposition::turnIntoNiceTreeDecomposition(Node_Id n_id) {
    const auto& node = nodes.at(n_id);
    for (Node_Id child_id : node.children) {
        const auto& child = nodes.at(child_id);
        assert(node.bag != child.bag);
    }

    if (node.children.size() == 1) {
        Node_Id child_id = *node.children.begin();
        bridgeDifference(n_id);
        turnIntoNiceTreeDecomposition(child_id);
    }
    else if (node.children.size() >= 2) {
        std::vector<Node_Id> node_children{node.children.begin(), node.children.end()};
        makeNJoinNodeNice(n_id);
        for (Node_Id child_id : node_children) {
            turnIntoNiceTreeDecomposition(child_id);
        }
    }
}

void TreeDecomposition::makeNodeNice(Node_Id n_id) {
    const auto& node = nodes.at(n_id);
    if (node.children.size() == 1)
        bridgeDifference(n_id);
    else if (node.children.size() >= 2)
        makeNJoinNodeNice(n_id);
}

void TreeDecomposition::bridgeDifference(const Node_Id parent_id) {
    const auto& parent_node = nodes.at(parent_id);
    assert(nodes[parent_id].children.size() == 1);

    Node_Id child_id = *parent_node.children.begin();
    const auto& child_node = nodes.at(child_id);

    const Bag extra_in_parent_bag = setDifferrence(parent_node.bag, child_node.bag);
    const Bag extra_in_child_bag = setDifferrence(child_node.bag, parent_node.bag);

    removeEdge(parent_id, child_id);

    Bag cur_bag = parent_node.bag;
    Node_Id cur_n_id = parent_id;

    assert(cur_bag == nodes[cur_n_id].bag); // invariant
    // Introduce nodes first (starting from the parent node)
    for (const Vertex_Id& v_id : extra_in_parent_bag) {
        cur_bag.erase(v_id);
        if (cur_bag == child_node.bag) {
            break;
        }
        
        Node_Id new_n_id = addNode();
        
        nodes[new_n_id].bag = cur_bag;
        addEdge(cur_n_id, new_n_id);

        cur_n_id = new_n_id; // cur_bag is already updated
        assert(cur_bag == nodes[cur_n_id].bag);
    }

    // Forget nodes second (starting from the parent node)
    for (const Vertex_Id& v_id : extra_in_child_bag) {
        cur_bag.insert(v_id);
        if (cur_bag == child_node.bag) {
            break;
        }
        Node_Id new_n_id = addNode();

        nodes[new_n_id].bag = cur_bag;
        addEdge(cur_n_id, new_n_id);

        cur_n_id = new_n_id; // cur_bag is already updated
        assert(cur_bag == nodes[cur_n_id].bag);
    }

    addEdge(cur_n_id, child_id);
}

void TreeDecomposition::makeNJoinNodeNice(Node_Id cur_n_id) {
    const auto& cur_node = nodes[cur_n_id];
    const std::vector<Node_Id> node_children{cur_node.children.begin(), cur_node.children.end()};
    assert(node_children.size() >= 2);

    for (Node_Id child_id : node_children) { // todo: remove assertion
        const auto& child = nodes.at(child_id);
        assert(cur_node.bag != child.bag);
    }

    std::vector<Node_Id> full_bag_nodes;
    Node_Id prev_new_node = cur_n_id;
    // Disconnect cur_n from its children
    for (Node_Id child_id : node_children) {
        removeEdge(cur_n_id, child_id);
    }

    // Create 2(!) new full bag nodes n-1 times where n is the number of children
    // and append those nodes as children to the last_node (init=cur_n_id)
    for (size_t i = 0; i < node_children.size() - 1; i++) {
        Node_Id new_node_left = addNode();
        Node_Id new_node_right = addNode();
        nodes[new_node_left].bag = cur_node.bag;
        nodes[new_node_right].bag = cur_node.bag;
        addEdge(prev_new_node, new_node_left);
        addEdge(prev_new_node, new_node_right);

        full_bag_nodes.push_back(new_node_left);
        prev_new_node = new_node_right;
    }
    full_bag_nodes.push_back(prev_new_node);
    auto count = std::count_if(full_bag_nodes.begin(), full_bag_nodes.end(), [prev_new_node](Node_Id n_id) {return n_id == prev_new_node;});
    assert(count == 1);

    assert(node_children.size() == full_bag_nodes.size());

    // Append the children to every full bag and bridge the difference to it
    for (size_t i = 0; i < node_children.size(); i++) {
        Node_Id full_bag_node = full_bag_nodes[i];
        Node_Id child_id = node_children[i];

        addEdge(full_bag_node, child_id);
        bridgeDifference(full_bag_node);
    }
}

std::string TreeDecomposition::printNodes(const std::vector<Node_Id>& n_ids) const {
    std::string str = "";
    for (auto n_id : n_ids) {
        str += idToName(n_id) + " ";
    }
    return str;
}

bool TreeDecomposition::isValid() const {
    // Property 1: Every vertex appears in some bag
    const std::vector<Vertex_Id> vertices = graph_ptr->getVertices();

    for (auto& v_id : vertices) {
        bool found = false;
        for (const auto& n_pair : nodes) {
            if (n_pair.second.bag.contains(v_id)) {
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }

    // Property 2: Every edge appears in some bag
    const std::vector<Edge>& edges = graph_ptr->getEdges();
    bool success = std::all_of(edges.begin(), edges.end(), [this](const Edge& edge){
        for (const std::pair<Node_Id, Node_Attributes>& node_pair : nodes) {
            if (contains(node_pair.second.bag, edge.first) && contains(node_pair.second.bag, edge.second))
                return true;
        }
        return false;
    });
    if (!success) {
        return false;
    }

    // Property 3: For every vertex v, the subtree containing vertex v in its bags is connected.
    success = std::all_of(vertices.begin(), vertices.end(), [this](const Vertex_Id v_id) {
        std::unordered_set<Node_Id> visited;
        std::stack<Node_Id> to_visit;
        bool found = false;
        Node_Id start;
        for (const auto& n_pair : nodes) {
            if (n_pair.second.bag.contains(v_id)) {
                found = true;
                start = n_pair.first;
                break;
            }
        }
        assert(found);
        to_visit.push(start);
        assert(nodes.at(start).bag.contains(v_id));
        while (!to_visit.empty()) {
            Node_Id cur_n_id = to_visit.top();
            const auto& node = nodes.at(cur_n_id);
            to_visit.pop();
            visited.insert(cur_n_id);
            for (const Node_Id& n_id : node.neighbours) {
                if (!(visited.contains(n_id)) && nodes.at(n_id).bag.contains(v_id)) {
                    to_visit.push(n_id);
                }
            }
        }

        std::unordered_set<Node_Id>compare;
        for (const auto& node : nodes) {
            if (node.second.bag.contains(v_id))
                compare.insert(node.first);
        }
        return visited == compare;
    });
    
    if (!success) {
        return false;
    }

    return true;
}

const std::unordered_set<TD_Edge>& TreeDecomposition::getEdges() const {
    return edges;
}

bool TreeDecomposition::areNeighbours(Node_Id n_id1, Node_Id n_id2) const {
    return nodes.at(n_id1).neighbours.contains(n_id2);
}

std::string TreeDecomposition::idToName(Node_Id n_id) const {
    return nodes.at(n_id).name;
}

Node_Id TreeDecomposition::nameToId(const std::string& name) const {
    return node_name_to_id.at(name);
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
    assert(!isRooted());

    auto it = nodes.begin();
    for (const auto& n_pair : nodes) {

    }
    auto node_with_largest_bag = std::max_element(nodes.begin(), nodes.end(), [](const auto& node1, const auto& node2) {
        return node1.second.bag.size() <= node2.second.bag.size();
    });

    rootTree(node_with_largest_bag->first);
    assert(isRooted());

    return node_with_largest_bag->first;
}

void TreeDecomposition::rootTree(Node_Id designated_root) {
    assert(!isRooted());

    std::vector<bool> visited(nodes.size(), false);

    std::stack<Node_Id> to_visit;
    to_visit.push(designated_root);

    while (!to_visit.empty()) {
        Node_Id cur_n_id = to_visit.top();
        Node_Attributes& cur_node = nodes[cur_n_id];
        to_visit.pop();
        visited[cur_n_id] = true;
        const std::unordered_set<Node_Id>& neighbours = cur_node.neighbours;
        
        for (const Node_Id& neighbour : neighbours) {
            if (visited[neighbour])
                continue; // has been visited already
            else {
                nodes[neighbour].parent = cur_n_id;
                cur_node.children.insert(neighbour);
                to_visit.push(neighbour);
            }
        }
    }
    root = designated_root;

    assert(isRooted());
}

const Node_Attributes& TreeDecomposition::getNode(Node_Id n_id) const {
    return nodes.at(n_id);
}

Node_Id TreeDecomposition::addNode() {
    return addNode("NEW_" + std::to_string(new_nodes_counter++));
}

Node_Id TreeDecomposition::addNode(const std::string &n_name)
{
    Node_Id new_id;
    if (!node_name_to_id.contains(n_name)) {
        Node_Attributes new_node;
        new_node.name = n_name;
        new_id = next_free_id++;
        
        node_name_to_id.insert({n_name, new_id});

        nodes.insert({new_id, new_node});
    }
    else {
        new_id = node_name_to_id.at(n_name);
    }

    assert(!nodes.empty());

    return new_id;
}

Node_Id TreeDecomposition::removeNode(Node_Id n_id) {
    assert(nodes.contains(n_id));

    nodes.erase(n_id);

    return -1;
}

void TreeDecomposition::addEdge(Node_Id n1_id, Node_Id n2_id) {
    assert(!areNeighbours(n1_id, n2_id));

    // Add edge in adjacencies
    nodes[n1_id].neighbours.insert(n2_id);
    nodes[n2_id].neighbours.insert(n1_id);

    // Add edge in edges
    edges.insert({std::min(n1_id, n2_id), std::max(n1_id, n2_id)});

    // If tree is rooted: Add edge in parents and children
    if (isRooted()) {
        nodes[n2_id].parent = n1_id;
        nodes[n1_id].children.insert(n2_id);
    }
}

void TreeDecomposition::removeEdge(Node_Id n1_id, Node_Id n2_id) {
    assert(areNeighbours(n1_id, n2_id));

    // Remove edge in adjacencies
    nodes[n1_id].neighbours.erase(n2_id);
    nodes[n2_id].neighbours.erase(n1_id);

    // Remove edge in edges
    edges.erase({std::min(n1_id, n2_id), std::max(n1_id, n2_id)});

    // If tree is rooted: Remove edge in parents and children
    if (isRooted()) {
        nodes[n2_id].parent = {};
        nodes[n1_id].children.erase(n2_id);
    }
}

string TreeDecomposition::printBag(const Bag& bag) const {
    string result_str = "";
    for (const Node_Id& n_id : bag) {
        result_str += graph_ptr->idToName(n_id) + " ";
    }
    return result_str;
}

string TreeDecomposition::printChildren(Node_Id n_id) const {
    string result_str = "";
    const auto& node = nodes.at(n_id);
    for (const Node_Id& child_id : node.children) {
        result_str += idToName(child_id) + " ";
    }
    return result_str;
}

void TreeDecomposition::doSomethingPreOrder(std::function<void(Node_Id)>f) const {
    doSomethingPreOrder(f, getRoot());
}

void TreeDecomposition::doSomethingPreOrder(std::function<void(Node_Id)>f, Node_Id n_id) const {
    f(n_id);
    const auto& node = nodes.at(n_id);
    const std::vector<Node_Id> children_copy{node.children.begin(), node.children.end()};
    for (const Node_Id& child_id : children_copy) {
        doSomethingPreOrder(f, child_id);
    }
}

void TreeDecomposition::doWhilePreOrder(std::function<void(Node_Id)> f, std::function<bool()> pred) const {
    doWhilePreOrder(f, pred, getRoot());
}

void TreeDecomposition::doWhilePreOrder(std::function<void(Node_Id)> f, std::function<bool()> pred, Node_Id n_id) const {
    if (!pred())
        return;

    f(n_id);
    for (const Node_Id& child_id : nodes.at(n_id).children) {
        doSomethingPreOrder(f, child_id);
    }
}

std::ostream &operator<<(std::ostream &stream, const TreeDecomposition &td)
{
    if (td.isRooted()) {
        stream << "Nodes (in pre-order):" << std::endl;
        td.doSomethingPreOrder([&stream, &td](const Node_Id& n_id) {
            // print node name, bag content, child names
            const auto& node = td.getNode(n_id);
            // const std::unordered_set<Node_Id>& children = td.getChildren(n_id);
            std::vector<std::string> children_names;
            for (const Node_Id child_id : node.children)
                children_names.push_back(td.idToName(child_id));
            std::vector<std::string> bag_names;
            for (const Vertex_Id v_id : node.bag)
                bag_names.push_back(td.graph_ptr->idToName(v_id));
            stream << td.idToName(n_id) << ": " << bag_names << ". Children: " << children_names << std::endl;
        });
    }
    else {
        stream << "Tree Decomposition with " << td.nodes.size() << " nodes. Those nodes are:" << std::endl;
        for (const auto& n_pair : td.nodes) {
            Node_Id n_id = n_pair.first;
            const auto& node = td.getNode(n_id);
            stream << td.idToName(n_id) << ": ";
            for (const Vertex_Id& v_id : node.bag) {
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

    }

    return stream;
}
