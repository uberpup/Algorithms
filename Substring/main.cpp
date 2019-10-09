/*
 Description (Russian): "Найдите все вхождения шаблона в строку.
    Длина шаблона – p, длина строки – n.
    Время O(n + p), доп. память – O(p).
    p <= 30000, n <= 300000."
 Time O(|pattern| + |text|)
 Memory O(|pattern|)
 */

#include <iostream>
#include <string>
#include <vector>

std::vector<size_t> OnlineOccurrenceIdx(const std::string& pattern_with_symbol);

size_t PrefixFunction(const std::string& pattern_with_symbol, char current_char,
                      const std::vector<size_t>& prefix_function_results, size_t prev_value);

std::vector<size_t> PrefixFunction(const std::string& text);

int main() {
    std::string pattern;

    std::cin >> pattern;
    auto result = OnlineOccurrenceIdx(pattern);
    for (const auto& idx : result){
        std::cout << idx << " ";
    }
    return 0;
}

std::vector<size_t> OnlineOccurrenceIdx(const std::string& pattern) {
    char in_char;
    auto prefix_function_results = PrefixFunction(pattern);
    std::vector<size_t> occurrence_idxes;

    size_t prev_value = 0;
    size_t counter = pattern.length();
    while (std::cin >> in_char) {
        prev_value = PrefixFunction(pattern,
                                    in_char,
                                    prefix_function_results, prev_value);
        if (prev_value ==
                pattern.length()) {
            occurrence_idxes.push_back(counter - 2 * pattern.length() + 1);
        }
        ++counter;
    }
    return occurrence_idxes;
}

size_t PrefixFunction(const std::string& pattern_with_symbol, char current_char,
        const std::vector<size_t>& prefix_function_results, size_t prev_value) {  // Преф. функция текста
    auto prefix_length = prev_value;
    while (prefix_length > 0 && current_char !=
        pattern_with_symbol[prefix_length]) {
        prefix_length = prefix_function_results[prefix_length - 1];
    }
    if (pattern_with_symbol[prefix_length] == current_char) {
        return ++prefix_length;
    }
    return prefix_length;
}

std::vector<size_t> PrefixFunction(const std::string& text) {  // O(n), для обработки паттерна
    std::vector<size_t> prefix_function_result(text.size());
    prefix_function_result[0] = 0;
    int prefix_length;
    for (size_t i = 1; i < text.length(); ++i) {
        prefix_length = prefix_function_result[i - 1];
        while (prefix_length > 0 && text[i] != text[prefix_length]) {
            prefix_length = prefix_function_result[prefix_length - 1];
        }
        if (text[i] == text[prefix_length]) {
            ++prefix_length;
        }
        prefix_function_result[i] = prefix_length;
    }
    return prefix_function_result;
}
