/*
 Даны строки s и t. Постройте сжатое суффиксное дерево,
 которое содержит все суффиксы строки s и строки t.
 Найдите такое дерево, которое содержит минимальное количество вершин.

 В основе алгоритм Укконена построения суффиксного дерева,
 https://stackoverflow.com/questions/9452701/ukkonens-suffix-tree-algorithm-in-plain-english/9513423#9513423

 Время: O(n*|sigma|)
 Память: O(n*|sigma|)
*/

#include <array>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

const char DIVIDER0 = '$';
const char DIVIDER1 = '#';
const size_t CHARSIZE = 256;

class SuffixTree {
public:
    SuffixTree();
    explicit SuffixTree(const std::string& str);

    struct Node {   // [start; end)
        Node();
        Node(size_t start, size_t end);
        size_t start;
        size_t end;
        size_t suff_link;
        size_t number;
        size_t transitions[CHARSIZE];

        static size_t position;

        size_t edge_length() {
            return std::min(end - start, position - start);
        }

        bool operator == (const Node& rhv) {
            return start == rhv.start && end == rhv.end &&
                   suff_link == rhv.suff_link && number == rhv.number;
        }
    };

    struct ActivePoint {
        ActivePoint();
        explicit ActivePoint(Node root);
        Node node;
        size_t edge;
        size_t len;
        ~ActivePoint() = default;
    };

    void AddSuffLink(const Node& from);

    void Descent(size_t node_number);

    void Insert(char ch);

    void Add(Node& node);

    size_t NodeCount();

    std::vector<std::array<size_t,4>> NodeData();

    void Search(size_t node_number, size_t parent_number, size_t& counter);

    ~SuffixTree() = default;

private:
    Node root;
    ActivePoint point;
    Node last;
    size_t remainder;
    size_t last_in_s;
    std::vector<char> txt;
    std::vector<Node> nodes;
    std::vector<std::array<size_t,4>> data;
};

int main() {
    std::string s;
    std::string t;
    std::cin >> s >> t;
    SuffixTree suff_tree(s + t);

    printf("%zu\n", suff_tree.NodeCount());
    auto output_data = suff_tree.NodeData();
    for (const auto& el : output_data) {
        printf("%zu %zu %zu %zu\n", el[0], el[1], el[2], el[3]);
    }
    return 0;
}


SuffixTree::SuffixTree() : nodes(), txt(),
                           remainder(0), root(), last(root), point(root) {}

SuffixTree::SuffixTree(const std::string& str): SuffixTree() {
    nodes.push_back(root);
    for (char ch : str) {
        txt.push_back(ch);
        Insert(ch);
        if (ch == DIVIDER0) {
            last_in_s = txt.size() - 1;
        }
        ++SuffixTree::Node::position;
    }
}

size_t SuffixTree::Node::position = 0;      // Изначально в root

SuffixTree::ActivePoint::ActivePoint(): node(), edge(0), len(0) {}

SuffixTree::ActivePoint::ActivePoint(Node root): node(root), edge(0), len(0) {}

SuffixTree::Node::Node(): start(0), end(UINT64_MAX), suff_link(0),
        number(0), transitions() {}

SuffixTree::Node::Node(size_t start, size_t end = UINT64_MAX): start(start), end(end),
        suff_link(0), number(0), transitions() {}

void SuffixTree::AddSuffLink(const Node& from) {
    if (last.number) {
        nodes[last.number].suff_link = from.number;
    }
    last = root;
}

void SuffixTree::Descent(size_t node_number) {  // Спуск по ребру
    point.edge += nodes[node_number].edge_length();
    point.len -= nodes[node_number].edge_length();
    point.node.number = node_number;
}

void SuffixTree::Add(Node& node) {
    nodes.push_back(node);
    node.number = nodes.size() - 1;
}

void SuffixTree::Insert(char ch) {
    ++remainder;
    last = root;
    while (remainder) {

        if (!point.len) {
            point.edge = SuffixTree::Node::position;
        }

        if (!nodes[point.node.number].transitions[txt[point.edge]]) {  // Правило #2

            Node leaf(SuffixTree::Node::position);
            Add(leaf);
            nodes[point.node.number].transitions[txt[point.edge]] = leaf.number;
            AddSuffLink(point.node);

        } else {
            size_t next = nodes[point.node.number].transitions[txt[point.edge]];

            if (point.len >= nodes[next].edge_length()) {
                Descent(next);
                continue;
            }

            if (txt[nodes[next].start + point.len] == ch) {
                AddSuffLink(point.node);
                ++point.len;
                break;
            }

            // Строим развилку
            Node split = Node(nodes[next].start, nodes[next].start + point.len);
            Add(split);
            nodes[point.node.number].transitions[txt[point.edge]] = split.number;

            Node leaf = Node(SuffixTree::Node::position);
            Add(leaf);
            nodes[split.number].transitions[ch] = leaf.number;

            nodes[next].start += point.len;
            nodes[split.number].transitions[txt[nodes[next].start]] = next;

            AddSuffLink(split);
        }

        --remainder;

        if (point.node == root && point.len) {        // Правило #1
            --point.len;
            point.edge = SuffixTree::Node::position - remainder + 1;
        } else if (point.node.number) {               // Правило #3
            point.node = nodes[nodes[point.node.number].suff_link];
        }
    }
}

size_t SuffixTree::NodeCount() {
    return nodes.size();
}

std::vector<std::array<size_t,4>> SuffixTree::NodeData() {
    size_t counter = 0;
    data.resize(NodeCount() - 1);
    for (auto& transition : nodes[0].transitions) {
        if (transition) {
            Search(transition, 0, counter);
        }
    }
    return data;
}

void SuffixTree::Search(size_t node_number, size_t parent_number, size_t& counter) {
    nodes[node_number].number = counter + 1;
    data[counter][0] = nodes[parent_number].number;

    if (nodes[node_number].end >= txt.size()) {
        nodes[node_number].end = txt.size() - 1;
    }

    if (nodes[node_number].start <= last_in_s) {
        data[counter][1] = 0;   // s
        data[counter][2] = nodes[node_number].start;
        data[counter][3] = std::min(nodes[node_number].end, last_in_s + 1);
    } else {
        data[counter][1] = 1;   // t
        data[counter][2] = nodes[node_number].start - last_in_s - 1;
        data[counter][3] = nodes[node_number].end - last_in_s;
    }

    ++counter;

    for (auto& transition : nodes[node_number].transitions) {
        if (transition) {
            Search(transition, node_number, counter);
        }
    }
}
