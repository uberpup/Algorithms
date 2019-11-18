/*
 * Даны два отрезка в пространстве (x1, y1, z1) - (x2, y2, z2) и
 * (x3, y3, z3) - (x4, y4, z4). Найдите расстояние между отрезками.
 * Ввод : Заданы целые x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4.
 * Координаты по модулю не превосходят 1000.
 * Вывод: Искомая величина c точностью не менее 6 знаков после десятичной точки.
 */

#include <iostream>
#include <ctgmath>
#include <iomanip>

double eps = 1e-10;

bool IsEqual(const double first, const double second) {
    return std::abs(first - second) <= eps;
}

struct PointR3 {
    double x;
    double y;
    double z;

    PointR3 operator + (const PointR3& rhv) {
        PointR3 result;
        result.x = x + rhv.x;
        result.y = y + rhv.y;
        result.z = z + rhv.z;
        return result;
    }
    PointR3 operator / (double rhv) {
        PointR3 result;
        result.x = x / rhv;
        result.y = y / rhv;
        result.z = z / rhv;
        return result;
    }
};

struct SegmentR3 {
    PointR3 start;     // Coordinates of start
    PointR3 end;     // Coordinates of end
};

double SegmentDistance(SegmentR3 seg_first, SegmentR3 seg_second);

double TernarySearchMin(PointR3 left, PointR3 right, const SegmentR3& segment);

double Distance(PointR3 p_first, PointR3 p_second);

double Distance(SegmentR3 segment, PointR3 point);

int main() {
    SegmentR3 seg_first {};
    SegmentR3 seg_second {};
    std::cin >> seg_first.start.x >> seg_first.start.y >> seg_first.start.z;
    std::cin >> seg_first.end.x >> seg_first.end.y >> seg_first.end.z;
    std::cin >> seg_second.start.x >> seg_second.start.y >> seg_second.start.z;
    std::cin >> seg_second.end.x >> seg_second.end.y >> seg_second.end.z;

    std::cout << std::setprecision(12) << SegmentDistance(seg_first, seg_second);
    return 0;
}


double SegmentDistance(SegmentR3 seg_first, SegmentR3 seg_second) {
    return TernarySearchMin(seg_first.start, seg_first.end, seg_second);
}

double TernarySearchMin(PointR3 left, PointR3 right, const SegmentR3& segment) {
    while (Distance(left, right) > eps) {
        auto left_third = (left + left + right) / 3;
        auto right_third = (left + right + right) / 3;
        if (Distance(segment, right_third) - Distance(segment, left_third) > eps) {
            right = right_third;
        } else {
            left = left_third;
        }
    }
    return Distance(segment, right);
}

double Distance(PointR3 p_first, PointR3 p_second) {
    return sqrt(pow(p_first.x - p_second.x, 2) + pow(p_first.y - p_second.y, 2)
            + pow(p_first.z - p_second.z, 2));
}

double Distance(SegmentR3 segment, PointR3 point) {
    if (IsEqual(segment.start.x, segment.end.x) && IsEqual(segment.start.y, segment.end.y)
            && IsEqual(segment.start.z, segment.end.z)) {      // If segment actually is a point
        PointR3 seg {segment.start.x, segment.start.y, segment.start.z};
        return Distance(seg, point);
    }
    // Formula for distance between dot and Segment
    double t = (point.x - segment.start.x) * (segment.end.x - segment.start.x);
    t += (point.y - segment.start.y) * (segment.end.y - segment.start.y);
    t += (point.z - segment.start.z) * (segment.end.z - segment.start.z);
    t /= (pow(segment.end.x - segment.start.x, 2)
            + pow(segment.end.y - segment.start.y, 2)
            + pow(segment.end.z - segment.start.z, 2));
    if (t <= eps) {
        t = 0;
    }
    if (t - 1 > eps) {
        t = 1;
    }
    double result = pow(segment.start.x - point.x
            + t * (segment.end.x - segment.start.x), 2);
    result += pow(segment.start.y - point.y
            + t * (segment.end.y - segment.start.y), 2);
    result += pow(segment.start.z - point.z
            + t * (segment.end.z - segment.start.z), 2);
    return sqrt(result);
}