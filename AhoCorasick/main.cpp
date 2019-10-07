/*
 Description(Russian): Шаблон поиска задан строкой длины m,
 в которой кроме обычных символов могут встречаться символы “?”.
 Найти позиции всех вхождений шаблона в тексте длины n.
 Каждое вхождение шаблона предполагает,
 что все обычные символы совпадают с соответствующими из текста,
 а вместо символа “?” в тексте встречается произвольный символ.
 m ≤ 5000, n ≤ 2000000.

 Время: O((n+m)log#Sigma + Z),
    log#Sigma - const, так что условие по требуемому времени не нарушается
 Память: O(m)
 */

#include <deque>
#include <queue>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

const char DIVIDER = '?';

class Bohr {
public:
    struct BohrNode {
        BohrNode();
        std::map<const char, std::shared_ptr<BohrNode>> transitions;
        std::weak_ptr<BohrNode> term_link;
        std::weak_ptr<BohrNode> suff_link;
        bool is_terminal;  // Если терминальная, то храним индекс внутри словаря
        bool is_root;
        size_t wordlist_index;

        bool operator ==(const BohrNode& rhv);
        std::shared_ptr<BohrNode> FindTransition(char ch);
        ~BohrNode() = default;
    };
    std::vector<std::pair<std::string, size_t>> wordlist;  // Строка и расстояние до начала
    // В случае вопросов в конце добавим фиктивную строку
    size_t pattern_size;
    BohrNode root;

    Bohr();
    Bohr(size_t pattern_size);
    void AddPattern(std::string pattern, size_t divider_count);
    void Init();
    void Step(const char ch, std::shared_ptr<BohrNode>& current_node);
    std::vector<size_t> PatternSearch(const std::string& text);
    ~Bohr() = default;
};

int main() {
    std::string pattern;
    std::cin >> pattern;
    Bohr bohr(pattern.length());

    size_t last_char = 0;
    size_t divider_count = 0;
    for (size_t i = 0; i < pattern.length(); ++i) {
        if (pattern[i] == DIVIDER) {  // Отдебажить бы
            if (!last_char && last_char != i) {
                bohr.AddPattern(pattern.substr(last_char, i), divider_count);
                divider_count = 0;
            }
            ++divider_count;
            last_char = i + 1;
        } else {
            divider_count = 0;
        }
    }
    if (divider_count) {  // Случай с вопросами в конце
        bohr.AddPattern("", divider_count);
    }
    bohr.Init();

    std::string text;
    std::cin >> text;
    auto result = bohr.PatternSearch(text);
    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << result[i] << " ";
    }

    return 0;
}

Bohr::BohrNode::BohrNode(): suff_link(), term_link(), transitions(),
        is_terminal(false), is_root(false),
        wordlist_index(-1) {}

Bohr::Bohr(): root(), wordlist(), pattern_size(0) {
    root.suff_link = std::make_shared<BohrNode>(root);  // Спецификация полей под корень
    root.term_link = root.suff_link;
    root.is_root = true;
}

Bohr::Bohr(size_t pattern_size): root(), wordlist(),
        pattern_size(pattern_size) {
    root.suff_link = std::make_shared<BohrNode>(root);  // Спецификация полей под корень
    root.term_link = root.suff_link;
    root.is_root = true;
}

bool Bohr::BohrNode::operator ==(const Bohr::BohrNode& rhv) {  // На всякий случай
    return transitions == rhv.transitions &&
            suff_link.lock() == rhv.suff_link.lock() &&
            term_link.lock() == rhv.term_link.lock() &&
            is_terminal == rhv.is_terminal &&
            is_root == rhv.is_root;
}

std::shared_ptr<Bohr::BohrNode> Bohr::BohrNode::FindTransition(char ch) {
    auto c_it = transitions.find(ch);
    if (c_it == transitions.cend()) {
        return nullptr;
    } else {
        return c_it->second;
    }
}

void Bohr::AddPattern(std::string pattern, size_t divider_count) {
    if (pattern.empty()) {
        wordlist.emplace_back(pattern, divider_count);
        return;
    }

    auto current_node = root;
    for (size_t i = 0; i < pattern.length(); ++i) {
        auto neighbour_node = current_node.FindTransition(pattern[i]);
        if (neighbour_node == nullptr) {
            neighbour_node = std::make_shared<BohrNode>();
            if (current_node.is_root) {  // Первые символы ссылаются на корень
                neighbour_node->suff_link = std::make_shared<BohrNode>(root);  // Does it work OK with weak_ptr?
                neighbour_node->term_link = neighbour_node->suff_link;
            }
            current_node.transitions[pattern[i]] = neighbour_node;
        }
        current_node = *neighbour_node;
    }
    if (!current_node.is_root) {
        current_node.is_terminal = true;
        current_node.wordlist_index = wordlist.size();
        if (wordlist.size() > 0) {
            wordlist.emplace_back(pattern,
                                  wordlist.back().second + pattern.length() + divider_count);
        } else {
            wordlist.emplace_back(pattern, pattern.length() + divider_count);
        }
    }
}

void Bohr::Init() {  // BFS
    std::queue<std::shared_ptr<BohrNode>> bfs_queue;
    bfs_queue.push(std::make_shared<BohrNode>(root));
    while (!bfs_queue.empty()) {
        auto& current_node = bfs_queue.front();
        bfs_queue.pop();
        for (auto& map_el : current_node->transitions) {  // Строим суффиксные и терминальные ссылки
            const char ch = map_el.first;
            auto& neighbour_node = map_el.second;

            auto& term_node = current_node->suff_link;
            while (!term_node.lock()->is_root) {  // Суффиксные
                if (term_node.lock()->FindTransition(ch) != nullptr) {
                    neighbour_node->suff_link = term_node.lock()->FindTransition(ch);
                    break;
                } else {
                    term_node = term_node.lock()->suff_link;
                }
            }
            if (neighbour_node->suff_link.lock() == nullptr) {
                neighbour_node->suff_link = std::make_shared<BohrNode>(root);
                neighbour_node->term_link = neighbour_node->suff_link;
            }

            if (neighbour_node->suff_link.lock()->is_terminal) {  // Терминальные
                neighbour_node->term_link = neighbour_node->suff_link;
            } else {
                neighbour_node->term_link =
                        neighbour_node->suff_link.lock()->term_link;
            }

            bfs_queue.push(neighbour_node);
        }
    }
}

void Bohr::Step(const char ch, std::shared_ptr<BohrNode>& current_node) {
    while (current_node != nullptr && !(current_node->is_root)) {
        auto candidate = current_node->FindTransition(ch);
        if (candidate != nullptr) {
            current_node = candidate;
            return;
        }
        current_node = current_node->suff_link.lock();
    }
    if (!(current_node->is_root)) {
        current_node = std::make_shared<BohrNode>(root);
    }
}

std::vector<size_t> Bohr::PatternSearch(const std::string& text) {  // Внимательно всмотреться в работу с вопросиками
    std::vector<size_t> pattern_indexes;
    std::deque<size_t> search_deque(pattern_size);
    size_t patterns_number = wordlist.size();

    auto current_node = std::make_shared<BohrNode>(root);
    for (size_t i = 0; i < text.length(); ++i) {
        Step(text[i], current_node);

        if (search_deque.front() == patterns_number) {  // Все слова из куска встретились
            pattern_indexes.push_back(i);
        }
        search_deque.pop_front();
        search_deque.push_back(0);

        while (current_node->is_terminal) {
            size_t dist = wordlist[current_node->wordlist_index].second;  // Расстояние до предполагаемого начала слова
            ++search_deque[search_deque.size() - dist];
            current_node = current_node->term_link.lock();
        }

    }
    return pattern_indexes;
}