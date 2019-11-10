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
const int ALPHASIZE = 'z' + 1;

class SuffixTree {
public:
    SuffixTree();
    explicit SuffixTree(const std::string& str);

    struct Node {   // [start; end]
        Node();
        //Node(int start, int end, int suff_link, int number);
        Node(int start, int end = INT32_MAX,
             int suff_link = 0, int number = 0);
        int start;
        int end;
        int suff_link;
        int number;
        int transitions[ALPHASIZE];

        static int position;

        int edge_length() {
            return std::min(end - start, position - start) + 1;
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
        int edge;
        int len;
        ~ActivePoint() = default;
    };

    void AddSuffLink(const Node& from);
    void AddSuffLink(const Node& from, const Node& to);

    void Descent(int node_number);

    void Insert(char ch);

    void Add(Node& node);

    int NodeCount();

    std::vector<std::array<int, 4>> NodeData();

    void Search(int node_number, int parent_number, int& counter);

    ~SuffixTree() = default;

private:
    Node root;
    ActivePoint point;
    Node last;
    Node dead_node;
    int remainder;
    int last_in_s;
    std::vector<char> txt;
    std::vector<Node> nodes;
    std::vector<std::array<int, 4>> data;
};

int main() {
    std::string s;
    std::string t;
    std::cin >> s >> t;
    SuffixTree suff_tree(s + t);

    printf("%zu\n", suff_tree.NodeCount());
    auto output_data = suff_tree.NodeData();
    for (const auto& el : output_data) {
        printf("%zu %zu %zu %zu\n", el[0], el[1], el[2], el[3] + 1);
    }
    return 0;
}


SuffixTree::SuffixTree() : root(-1, -1, -1), point(root), nodes(), last(root),
        txt(), dead_node(-1,-1,-1,-1), remainder(0) {}

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

int SuffixTree::Node::position = 0;      // Изначально в root

SuffixTree::ActivePoint::ActivePoint(): node(), edge(0), len(0) {}

SuffixTree::ActivePoint::ActivePoint(Node root): node(root), edge(0), len(0) {}

SuffixTree::Node::Node(): start(0), end(INT32_MAX), suff_link(0),
        number(0), transitions() {}

SuffixTree::Node::Node(int start, int end,
        int suff_link, int number): start(start), end(end),
        suff_link(suff_link), number(number), transitions() {}

void SuffixTree::AddSuffLink(const Node& from) {
    if (last.number > -1) {
        nodes[last.number].suff_link = from.number;
    }
    last = dead_node;
}

void SuffixTree::AddSuffLink(const Node& from, const Node& to) {
    if (last.number > -1) {
        nodes[last.number].suff_link = from.number;
    }
    last = to;
}

void SuffixTree::Descent(int node_number) {  // Спуск по ребру
    point.edge += nodes[node_number].edge_length();
    point.len -= nodes[node_number].edge_length();
    point.node.number = node_number;
}

void SuffixTree::Add(Node& node) {
    node.number = nodes.size();
    nodes.push_back(node);
}

void SuffixTree::Insert(char ch) {
    ++remainder;
    last = dead_node;
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
            int next = nodes[point.node.number].transitions[txt[point.edge]];

            if (point.len >= nodes[next].edge_length()) {
                Descent(next);
                continue;
            }

            if (txt[nodes[next].start + point.len] == ch) {
                if (point.node.number) {
                    AddSuffLink(point.node);
                }
                ++point.len;
                break;
            }

            // Строим развилку
            Node split = Node(nodes[next].start, nodes[next].start + point.len - 1);
            Add(split);
            nodes[point.node.number].transitions[txt[point.edge]] = split.number;

            Node leaf = Node(SuffixTree::Node::position);
            Add(leaf);
            nodes[split.number].transitions[ch] = leaf.number;

            nodes[next].start += point.len;
            nodes[split.number].transitions[txt[nodes[next].start]] = next;

            AddSuffLink(split, split);
        }

        --remainder;

        if (!point.node.number && point.len) {        // Правило #1
            --point.len;
            point.edge = SuffixTree::Node::position - remainder + 1;
        } else if (point.node.number) {               // Правило #3
            point.node = nodes[nodes[point.node.number].suff_link];
        }
    }
}

int SuffixTree::NodeCount() {
    return nodes.size();
}

std::vector<std::array<int, 4>> SuffixTree::NodeData() {
    int counter = 0;
    data.resize(NodeCount() - 1);
    for (auto& transition : nodes[0].transitions) {
        if (transition) {
            Search(transition, 0, counter);
        }
    }
    return data;
}

void SuffixTree::Search(int node_number, int parent_number, int& counter) {
    nodes[node_number].number = counter + 1;
    data[counter][0] = nodes[parent_number].number;

    if (nodes[node_number].end >= txt.size()) {
        nodes[node_number].end = txt.size() - 1;
    }

    if (nodes[node_number].start <= last_in_s) {
        data[counter][1] = 0;   // s
        data[counter][2] = nodes[node_number].start;
        data[counter][3] = std::min(nodes[node_number].end, last_in_s);
    } else {
        data[counter][1] = 1;   // t
        data[counter][2] = nodes[node_number].start - last_in_s - 1;
        data[counter][3] = nodes[node_number].end - last_in_s - 1;
    }

    ++counter;

    for (auto& transition : nodes[node_number].transitions) {
        if (transition) {
            Search(transition, node_number, counter);
        }
    }
}
