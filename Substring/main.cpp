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

const char SPECIAL_SYMBOL = '#';  // Unused as a symbol in pattern/text

std::vector<size_t> OccurrenceIdx(size_t pattern_length,
        const std::string& text);

std::vector<size_t> PrefixFunction(const std::string& text);

int main() {
    std::string pattern, text;

    std::cin >> pattern;
    std::cin >> text;
    text = pattern + SPECIAL_SYMBOL + text;  // Adding pattern and symbol
    auto result = OccurrenceIdx(pattern.size(), text);
    for (const auto& idx : result){
        std::cout << idx << " ";
    }
    return 0;
}

std::vector<size_t> OccurrenceIdx(size_t pattern_length,
        const std::string& text) {
    std::vector<size_t> prefix_function_result = PrefixFunction(text);
    std::vector<size_t> result;
    for (size_t i = pattern_length; i < text.size(); ++i) {
        if (prefix_function_result[i] == pattern_length) {  // Substr found
            result.push_back(i - 2 * pattern_length);  // Fixing the indexes
        }
    }
    return result;
}

std::vector<size_t> PrefixFunction(const std::string& text) {  // O(n)
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
