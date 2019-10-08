/*
 * Description(Russian):
 * 1.Найти лексикографически-минимальную строку, построенную по префикс-функции, в алфавите a-z.
 * 2.Найти лексикографически-минимальную строку, построенную по z-функции, в алфавите a-z.
 * Time: O(n)
 * Memory: O(n)
 */

#include <iostream>
#include <string>
#include <vector>
#include <stack>

std::vector<size_t> ZFunctionFromString(const std::string& str);

std::vector<size_t> PrefixFunctionFromZ(const std::vector<size_t>& z_func);

std::vector<char> StringFromPrefix(const std::vector<size_t>& prefix_func);

int main() {
    std::vector<size_t> z_func;
    std::string input;
    size_t val = 1;
    while (std::cin >> val) {
        z_func.push_back(val);
    }

    auto result = StringFromPrefix(PrefixFunctionFromZ(z_func));
    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << result[i];
    }
    return 0;
}

std::vector<size_t> ZFunctionFromString(const std::string& str) {
    std::vector<size_t> z_func(str.length());
    size_t left = 0;
    size_t right = 0;
    for (size_t i = 0; i < str.length(); ++i) {
        z_func[i] = 0;
        if (std::min(right - i, z_func[i - left]) > 0) {
            z_func[i] = std::min(right - i, z_func[i - left]);
        }
        while (i + z_func[i] < str.length() && str[z_func[i]] == str[i + z_func[i]]) {
            ++z_func[i];
        }
        if (i + z_func[i] > right) {
            left = i;
            right = i + z_func[i];
        }
    }
    return z_func;
}

std::vector<size_t> PrefixFunctionFromZ(const std::vector<size_t>& z_func) {
    std::vector<size_t> pref_func(z_func.size());
    for (size_t i = 1; i < z_func.size(); ++i) {
        for (long long j = z_func[i] - 1; j >= 0; --j) {
            if (pref_func[i + j] > 0) {
                break;
            } else {
                pref_func[i + j] = j + 1;
            }
        }
    }
    return pref_func;
}

std::vector<char> StringFromPrefix(const std::vector<size_t>& prefix_func) {
    std::vector<char> vector_str(prefix_func.size());
    std::stack<size_t> idx_stack;
    vector_str[0] = 'a';
    for (size_t i = 1; i < prefix_func.size(); ++i) {
        if (prefix_func[i] == 0) {
            size_t idx = i - 1;
            while (prefix_func[idx] != 0) {
                idx_stack.push(prefix_func[idx]);
                idx = prefix_func[idx] - 1;
            }
            char next_char = 'b';  // Очевидно не 'a'
            while (!idx_stack.empty()) {
                while (next_char == vector_str[idx_stack.top()]) {
                    ++next_char;  // Используем расположение в мин л-граф порядке в таблице символов
                }
                idx_stack.pop();
            }
            vector_str[i] = next_char;
        } else {
            vector_str[i] = vector_str[prefix_func[i] - 1];
        }
    }
    return vector_str;
}