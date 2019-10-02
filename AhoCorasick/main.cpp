#include <bits/shared_ptr.h>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

class Bohr {
public:
    struct BohrNode {
        BohrNode();
        std::map<char, BohrNode> transitions;
        // std::unordered_map<char, std::shared_ptr<BohrNode>> transitions_cache;
        std::shared_ptr<BohrNode> term_link;
        std::shared_ptr<BohrNode> suff_link;
        bool is_terminal;
        ~BohrNode();
    };
    std::vector<std::string_view> wordlist;
    BohrNode root;
public:
    Bohr();
    void AddPattern(std::string_view pattern);
    void Search(std::string_view pattern);
    std::vector<size_t> PositionsOfPatterns(std::string_view text);
    std::shared_ptr<BohrNode> state;
    ~Bohr();
};

int main() {
    Bohr bohr;
    std::string pattern;
    std::cin >> pattern;

    size_t last_char = 0;
    for (size_t i = 0; i < pattern.length(); ++i) {
        if (pattern[i] == '?') {
            bohr.AddPattern(pattern.substr(last_char, i));
            last_char = i + 1;
        }
    }

    std::string text;
    std::cin >> text;
    auto result = bohr.PositionsOfPatterns(text);
    for (int i = 0; i < result.size(); ++i){
        std::cout << result[i] << " ";
    }

    return 0;
}

Bohr::BohrNode::BohrNode(): suff_link(nullptr, nullptr),
        term_link(nullptr, nullptr), transitions(), is_terminal(false) {}

Bohr::Bohr(): state(nullptr, nullptr), root(), wordlist() {}  // Weird thing w/ BohrNode ptrs
