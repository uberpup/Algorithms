/*
 * Даны n точек в пространстве.
 * Никакие 4 точки не лежат в одной плоскости.
 * Найдите выпуклую оболочку этих точек за O(n*log(n)).
 * Ввод: Первая строка содержит число m - количество тестов.
 * В последующих строках описаны сами тесты. Каждый тест начинается со строки, содержащей n (n ≤ 1000) - число точек.
 * Далее, в n строках даны по три числа - координаты точек. Все координаты целые, не превосходят по модулю 500.
 * Вывод: Для каждого теста выведите следующее. В первую строку выведите количество граней m.
 * Далее в последующие m строк выведите описание граней: количество точек грани (=3) и номера точек в исходном множестве.
 * Точки нумеруются в том же порядке, в котором они даны во входном файле.
 * Точки в пределах грани должны быть отсортированы в порядке против часовой стрелки относительно внешней нормали к грани.
 * Первая точка – точка с минимальным номером. Порядок граней лексикографический.
 *
 * Реализация опирается на данный алгоритм: http://tmc.web.engr.illinois.edu/ch3d/ch3d.pdf
 */

#include <algorithm>
#include <cassert>
#include <ctgmath>
#include <iostream>
#include <list>
#include <tuple>
#include <vector>

const double rot_eps = 1e-3;
const double cmp_eps = 1e-9;
const double inf = std::numeric_limits<double>::infinity();
const int EVENTS = 6;

enum PositioningCases {
    POINT_LEFT_OF_u = 0,
    POINT_RIGHT_OF_v = 1,
    BRIDGE_Uleft_V = 2,
    BRIDGE_Uright_V = 3,
    BRIDGE_U_Vright = 4,
    BRIDGE_U_Vleft = 5
};

bool IsEqual(const double first, const double second) {
    return std::abs(first - second) <= cmp_eps;
}

struct PointR3 {
    PointR3() = default;
    PointR3(double x, double y, double z) : x(x), y(y), z(z),
            prev(nullptr), next(nullptr) {};
    PointR3(double x, double y, double z, int id) : x(x), y(y), z(z), id(id),
            prev(nullptr), next(nullptr) {};
    PointR3(double x, double y, double z, PointR3* prev, PointR3* next) : x(x),
            y(y), z(z), prev(prev), next(next) {};
    PointR3(double x, double y, double z, int id, PointR3* prev, PointR3* next) :
            x(x), y(y), z(z), id(id), prev(prev), next(next) {};

    double x;
    double y;
    double z;

    PointR3* prev;
    PointR3* next;

    int id = -1;

    bool Act() {
        if (prev->next != this) {  // inserting
            prev->next = next->prev = this;
            return true;
        } else {                  // deleting
            prev->next = next;
            next->prev = prev;
            return false;
        }
    }
};

struct Facet {
    int x;
    int y;
    int z;
};

PointR3 null_point = {inf, inf, inf, nullptr, nullptr};
PointR3* null_event = &null_point;

struct Hull {
    void SortFacets() {
        for (auto& facet : facets) {
            if (facet.y < facet.x && facet.y < facet.z) {
                std::swap(facet.x, facet.y);
                std::swap(facet.y, facet.z);
            } else if (facet.z < facet.x && facet.z < facet.y) {
                std::swap(facet.y, facet.z);
                std::swap(facet.x, facet.y);
            }
        }
        std::stable_sort(facets.begin(), facets.end(), [](const Facet& first, const Facet& second) {
                return std::tie(first.x, first.y, first.z) <
                        std::tie(second.x, second.y, second.z); });
    };

    std::vector<Facet> facets;
};

void Rotate(double& coord_1, double& coord_2, const double angle);

double Turn(const PointR3& p, const PointR3& q, const PointR3& r);

double Turn(const PointR3* p, const PointR3* q, const PointR3* r);

double Time(const PointR3& p, const PointR3& q, const PointR3& r);

double Time(const PointR3* p, const PointR3* q, const PointR3* r);

void HalfHull(std::vector<PointR3>& points, Hull& hull, bool is_lower);

std::vector<PointR3*> GetEvents(std::vector<PointR3>& points,
                                const size_t begin_idx, const size_t end_idx);

struct PointSet {
    void RotateAndAdd(double x, double y, double z, int id) {
        Rotate(z, y, rot_eps);
        Rotate(x, z, rot_eps);
        Rotate(x, y, rot_eps);
        points.emplace_back(x, y, z, id);
    };
    void FlipZ() {
        for (auto& point : points) {
            point.z = -point.z;
            point.prev = nullptr;
            point.next = nullptr;
        }
    }
    Hull MakeConvexHull();
    std::vector<PointR3> points;
};


int main() {
    size_t tests;
    size_t points_count;
    std::cin >> tests;
    for (size_t i = 0; i < tests; ++i) {
        std::cin >> points_count;
        PointSet ps;
        for (size_t j = 0; j < points_count; ++j) {
            int x, y, z;
            std::cin >> x >> y >> z;
            ps.RotateAndAdd(x, y, z, j);
        }
        Hull hull = ps.MakeConvexHull();
        hull.SortFacets();
        std::cout << hull.facets.size() << std::endl;
        for (const auto& facet : hull.facets) {
            std::cout << "3 " << facet.x << " " << facet.y << " "
                    << facet.z << std::endl;
        }
    }
    return 0;
}


void Rotate(double& coord_1, double& coord_2, const double angle) {
    double cos_ = cos(angle);
    double sin_ = sin(angle);
    double coord_1_new = coord_1 * cos_ + coord_2 * sin_;
    double coord_2_new = -coord_1 * sin_ + coord_2 * cos_;
    coord_1 = coord_1_new;
    coord_2 = coord_2_new;
}

double Turn(const PointR3& p, const PointR3& q, const PointR3& r) {
    return (q.x - p.x) * (r.y - p.y) - (r.x - p.x) * (q.y - p.y);
}

double Turn(const PointR3* p, const PointR3* q, const PointR3* r) {
    if (p == null_event || q == null_event || r == null_event) {
        return 1;
    }
    if (p == nullptr || q == nullptr || r == nullptr) {
        return 1;
    }
    return Turn(*p, *q, *r);
}

double Time(const PointR3& p, const PointR3& q, const PointR3& r) {
    return ((q.x - p.x) * (r.z - p.z) - (r.x - p.x) * (q.z - p.z)) /
           Turn(p, q, r);
}

double Time(const PointR3* p, const PointR3* q, const PointR3* r) {
    if (p == null_event || q == null_event || r == null_event) {
        return inf;
    }
    if (p == nullptr || q == nullptr || r == nullptr) {
        return inf;
    }
    return Time(*p, *q, *r);
}

Hull PointSet::MakeConvexHull() {
    std::stable_sort(points.begin(), points.end(),      // sorting points by x coordinate
                     [](const PointR3& first, const PointR3& second) {
                         return std::tie(first.x, first.y, first.z) <
                                std::tie(second.x, second.y, second.z); });
    Hull res_hull;
    HalfHull(points, res_hull, true);
    FlipZ();
    HalfHull(points, res_hull, false);
    return res_hull;
}

void HalfHull(std::vector<PointR3>& points, Hull& hull, bool is_lower) {
    if (points.size() == 0) {
        return;
    }
    if (points.size() == 1) {
        points[0].prev = &null_point;
        points[0].next = &null_point;
        return;
    }

    std::vector<PointR3*> events = GetEvents(points, 0, points.size());
    for (auto event : events) {
        Facet current = { event->prev->id, event->id, event->next->id };
        if (is_lower == !event->Act()) {
            std::swap(current.x, current.y);
        }
        hull.facets.push_back(current);
    }
}

std::vector<PointR3*> GetEvents(std::vector<PointR3>& points,
                                const size_t begin_idx, const size_t end_idx) {
    if (end_idx - begin_idx == 1) {
        return {};
    }
    const size_t middle_idx = (begin_idx + end_idx) / 2;
    std::vector<PointR3*> events;
    std::vector<PointR3*> events_buff[2] = {
            GetEvents(points, begin_idx, middle_idx),
            GetEvents(points, middle_idx, end_idx)
    };

    PointR3* u = &points[middle_idx - 1];
    PointR3* v = &points[middle_idx];

    while (Turn(u, v, v->next) < 0 || Turn(u->prev, u, v) < 0) {    // initial bridge
        if (Turn(u->prev, u, v) < 0) {
            u = u->prev;
        }
        else if (Turn(u, v, v->next) < 0) {
            v = v->next;
        }
    }

    size_t event_1 = 0;
    size_t event_2 = 0;
    double cur_time = -inf;

    while (true) {
        PointR3* left_point = &null_point;
        PointR3* right_point = &null_point;
        double event_time[EVENTS];

        if (event_1 < events_buff[0].size()) {
            left_point = events_buff[0][event_1];
            event_time[0] = Time(left_point->prev, left_point, left_point->next);
        } else {
            event_time[0] = inf;
        }

        if (event_2 < events_buff[1].size()) {
            right_point = events_buff[1][event_2];
            event_time[1] = Time(right_point->prev, right_point, right_point->next);
        } else {
            event_time[1] = inf;
        }

        event_time[2] = Time(u, v, v->next);
        event_time[3] = Time(u, v->prev, v);
        event_time[4] = Time(u->prev, u, v);
        event_time[5] = Time(u, u->next, v);

        size_t new_time_idx = EVENTS + 1;
        double new_time = inf;
        for (int i = 0; i < EVENTS; i++) {
            if (event_time[i] - cur_time > cmp_eps &&
                    new_time - event_time[i] > cmp_eps) {
                new_time = event_time[i];
                new_time_idx = i;
            }
        }

        if (new_time_idx == EVENTS + 1) {
            break;
        }

        switch (new_time_idx) {
            case POINT_LEFT_OF_u:
                if (u->x - left_point->x > cmp_eps) {
                    events.push_back(left_point);
                }
                left_point->Act();
                event_1++;
                break;
            case POINT_RIGHT_OF_v:
                if (right_point->x - v->x > cmp_eps) {
                    events.push_back(right_point);
                }
                right_point->Act();
                event_2++;
                break;
            case BRIDGE_Uleft_V:
                events.push_back(v);
                v = v->next;
                break;
            case BRIDGE_Uright_V:
                v = v->prev;
                events.push_back(v);
                break;
            case BRIDGE_U_Vright:
                events.push_back(u);
                u = u->prev;
                break;
            case BRIDGE_U_Vleft:
                u = u->next;
                events.push_back(u);
                break;
            default:
                assert(false);
                break;
        }
        cur_time = new_time;
    }
    u->next = v;
    v->prev = u;

    // Updating pointers by reverse traverse
    for (auto i = static_cast<int64_t>(events.size() - 1); i >= 0; --i) {
        PointR3* event = events[i];
        if (event->x - u->x > cmp_eps && v->x - event->x > cmp_eps) {
            u->next = event;
            v->prev = event;
            event->prev = u;
            event->next = v;
            if (points[middle_idx - 1].x - event->x < cmp_eps) {
                u = event;
            } else {
                v = event;
            }
        } else {
            event->Act();
            if (event == u) {
                u = u->prev;
            }
            if (event == v) {
                v = v->next;
            }
        }
    }
    return events;
}
