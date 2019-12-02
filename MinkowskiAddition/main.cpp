/*
 * Даны два выпуклых многоугольника на плоскости.
 * В первом n точек, во втором m.
 * Определите, пересекаются ли они за O(n + m).
 * Указание - используйте сумму Минковского.
 * Ввод : n, n точек первого многоугольника, отсортированные по часовой стрелке.
 * m, m точек второго многоугольника, отсортированные по часовой точке.
 * Вывод: YES/NO
 */

#include <algorithm>
#include <ctgmath>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <vector>

double eps = 1e-10;

bool IsEqual(const double first, const double second) {
    return std::abs(first - second) <= eps;
}

struct PointR2 {
    double x;
    double y;
    PointR2() = default;
    PointR2(double x, double y) : x(x), y(y) {}
    PointR2 operator + (const PointR2& rhv) {
        PointR2 result;
        result.x = x + rhv.x;
        result.y = y + rhv.y;
        return result;
    }
    PointR2 operator / (double rhv) {
        PointR2 result;
        result.x = x / rhv;
        result.y = y / rhv;
        return result;
    }

};

PointR2 operator - (const PointR2& first, const PointR2& second) {
    PointR2 result;
    result.x = first.x - second.x;
    result.y = first.y - second.y;
    return result;
}

struct Polygon {
    Polygon() : vertices_count(0), vertices() {}
    size_t vertices_count;
    std::vector<PointR2> vertices;
};

bool DoIntersect(Polygon poly, Polygon contr_poly);     // Does the addition contain (0, 0)

bool IsInside(PointR2 point, const Polygon& poly);

int CompareAngles(const PointR2& x0, const PointR2& y0, const PointR2& x1, const PointR2& y1);

void CyclicShift(std::vector<PointR2>& vector);

int main() {
    size_t n, m;
    Polygon poly, contr_poly;
    std::cin >> n;
    poly.vertices_count = n;
    for (size_t i = 0; i < n; ++i) {
        double x, y;
        std::cin >> x >> y;
        poly.vertices.emplace_back(x, y);
    }
    std::cin >> m;
    contr_poly.vertices_count = m;
    for (size_t i = 0; i < m; ++i) {    // Making contrary polygon
        double x, y;
        std::cin >> x >> y;
        contr_poly.vertices.emplace_back(-x, -y);
    }
    if (DoIntersect(poly, contr_poly)) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }
    return 0;
}

bool IsInside(PointR2 point, const Polygon& poly) {
    double det;
    for (size_t i = 0; i < poly.vertices.size() - 1; ++i) {
        det = poly.vertices[i].x * poly.vertices[i + 1].y -
              poly.vertices[i + 1].x * poly.vertices[i].y;
        if (det < -eps) {
            return false;
        }
    }
    det = poly.vertices.back().x * poly.vertices[0].y -
          poly.vertices[0].x * poly.vertices.back().y;
    return !(det < -eps);
}

int CompareAngles(const PointR2& x0, const PointR2& y0, const PointR2& x1, const PointR2& y1) {
    PointR2 first = y0 - x0;
    PointR2 second = x1 - y1;
    double det = first.x * second.y - second.x * first.y;
    if (IsEqual(det, 0)) {
        return 0;
    } else if (det < 0) {
        return -1;
    } else {
        return 1;
    }
}

bool DoIntersect(Polygon poly, Polygon contr_poly) {
    Polygon summed_poly;
    std::reverse(poly.vertices.begin(), poly.vertices.end());
    std::reverse(contr_poly.vertices.begin(), contr_poly.vertices.end());
    CyclicShift(poly.vertices);
    CyclicShift(contr_poly.vertices);
    poly.vertices.push_back(poly.vertices[0]);
    contr_poly.vertices.push_back(contr_poly.vertices[0]);

    size_t i = 0;
    size_t j = 0;
    while (i < poly.vertices_count && j < contr_poly.vertices_count) {
        summed_poly.vertices.push_back(poly.vertices[i] + contr_poly.vertices[j]);
        int result = CompareAngles(poly.vertices[i], poly.vertices[i + 1],
                                   contr_poly.vertices[j], contr_poly.vertices[j + 1]);
        if (result == 1) {
            ++j;
        } else if (result == -1) {
            ++i;
        } else {
            ++i;
            ++j;
        }
    }
    summed_poly.vertices_count = summed_poly.vertices.size();
    return IsInside({0, 0}, summed_poly);
}

void CyclicShift(std::vector<PointR2>& vector) {
    size_t dist = std::distance(vector.begin(), std::min_element(vector.begin(), vector.end(),
            [](const PointR2& first, const PointR2& second) {
                    return std::tie(first.y, first.x) <
                            std::tie(second.y, second.x); }));
    std::vector<PointR2> buf(vector.begin(), vector.begin() + dist);
    vector = std::vector<PointR2>(vector.begin() + dist, vector.end());
    std::copy(buf.begin(), buf.end(), std::back_inserter(vector));
}