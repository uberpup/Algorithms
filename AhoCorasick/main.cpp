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
        std::vector<size_t> wordlist_idxs;  // Можно сделать с-шным массивом, если будут проблемы по времени

        bool operator ==(const BohrNode& rhv);
        std::shared_ptr<BohrNode> FindTransition(char ch);
        ~BohrNode() = default;
    };
    std::vector<std::pair<std::string, size_t>> wordlist;  // Строка и расстояние до начала
    // В случае вопросов в конце добавим фиктивную строку
    size_t pattern_size;
    std::shared_ptr<BohrNode> root;

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
            if (last_char != i) {
                bohr.AddPattern(pattern.substr(last_char, i - last_char),
                        divider_count);
                divider_count = 0;
            }
            ++divider_count;
            last_char = i + 1;
        } else if (i == pattern.length() - 1) {
            bohr.AddPattern(pattern.substr(last_char, i - last_char + 1),
                    divider_count);
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
                            wordlist_idxs() {}

Bohr::Bohr(): root(std::make_shared<BohrNode>()), wordlist(), pattern_size(0) {
    root->suff_link = std::weak_ptr<BohrNode>(root);  // Спецификация полей под корень
    root->term_link = std::weak_ptr<BohrNode>(root);
    root->is_root = true;
}

Bohr::Bohr(size_t pattern_size): root(std::make_shared<BohrNode>()), wordlist(),
        pattern_size(pattern_size) {
    root->suff_link = std::weak_ptr<BohrNode>(root);  // Спецификация полей под корень
    root->term_link = std::weak_ptr<BohrNode>(root);
    root->is_root = true;
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
        auto neighbour_node = current_node->FindTransition(pattern[i]);
        if (neighbour_node == nullptr) {
            neighbour_node = std::make_shared<BohrNode>();
            if (current_node->is_root) {  // Первые символы ссылаются на корень
                neighbour_node->suff_link = std::weak_ptr<BohrNode>(root);
                neighbour_node->term_link = neighbour_node->suff_link;
            }
            current_node->transitions[pattern[i]] = neighbour_node;
        }
        current_node = neighbour_node;
    }

    if (!current_node->is_root) {
        current_node->is_terminal = true;
        current_node->wordlist_idxs.push_back(wordlist.size());
        wordlist.emplace_back(pattern,
                              (!wordlist.empty() ?
                              wordlist.back().second + pattern.length() :
                              pattern.length()) + divider_count);
    }
}

void Bohr::Init() {  // BFS
    std::queue<std::shared_ptr<BohrNode>> bfs_queue;  // Куда пропали переходы?
    bfs_queue.push(root);
    while (!bfs_queue.empty()) {
        auto current_node = bfs_queue.front();
        bfs_queue.pop();
        for (auto& map_el : current_node->transitions) {  // Строим суффиксные и терминальные ссылки
            const char ch = map_el.first;
            auto neighbour_node = map_el.second;

            auto term_node = current_node->suff_link;
            while (!term_node.lock()->is_root) {  // Суффиксные
                if (term_node.lock()->FindTransition(ch) != nullptr) {
                    neighbour_node->suff_link = term_node.lock()->FindTransition(ch);
                    break;
                } else {
                    term_node = term_node.lock()->suff_link;
                }
            }
            if (neighbour_node->suff_link.lock() == nullptr) {
                neighbour_node->suff_link = std::weak_ptr<BohrNode>(root);
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
    while (current_node != nullptr) {
        auto candidate = current_node->FindTransition(ch);
        if (candidate != nullptr) {
            current_node = candidate;
            return;
        }
        if (candidate == nullptr && current_node == root) {
            return;
        }
        current_node = current_node->suff_link.lock();
    }

    if (!(current_node->is_root)) {  // Если никого не нашли
        current_node = root;
    }
}

std::vector<size_t> Bohr::PatternSearch(const std::string& text) {  // Проверка вопросов в конце
    std::vector<size_t> pattern_indexes;
    std::deque<size_t> search_deque(pattern_size);
    size_t patterns_number = wordlist.size();

    auto current_node = root;
    for (size_t i = 0; i < text.length(); ++i) {
        Step(text[i], current_node);
        auto additional_node = current_node;
        while (!additional_node->is_root) {
            if (additional_node->is_terminal) {
                for (const auto& idx : additional_node->wordlist_idxs) {
                    // Вычитаем из размера расстояние до предполагаемого начала слова
                    ++search_deque[search_deque.size() - wordlist[idx].second];
                }
            }
            additional_node = additional_node->term_link.lock();
        }

        if (search_deque.front() == patterns_number) {  // Все слова из куска встретились
            pattern_indexes.push_back(i);
        }
        search_deque.pop_front();
        search_deque.push_back(0);

    }
    return pattern_indexes;
}