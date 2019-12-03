/*
 * Даны точки, никакие 3 из которых не лежат на одной прямой. Никакие 4 точки не лежат на одной окружности. Кроме того, все точки имеют различные x-координаты. Определите среднее число сторон в многоугольниках диаграммы Вороного этого множества точек. Считаются только конечные многоугольники.
 * Если все многоугольники неограниченны, ответ полагается равным 0.
 * Число точек n ≤ 100000. Алгоритм должен иметь асимптотику O(n log n)
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
#include <vector>
#include <tuple>
#include <iomanip>

const double eps = 1e-7;
const double inf = std::numeric_limits<double>::infinity();
const int EVENTS = 6;

struct PointR3 {
    PointR3() = default;
    PointR3(double x, double y, double z) : x(x), y(y), z(z),
                                            prev(nullptr), next(nullptr) {};
    PointR3(double x, double y, double z, int id) : x(x), y(y), z(z), id(id),
                                                    prev(nullptr), next(nullptr) {};
    PointR3(double x, double y, double z, PointR3* prev, PointR3* next) : x(x),
                                                                          y(y), z(z), prev(prev), next(next) {};
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
    bool visited = false;
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

void Rotate(double& coord_1, double& coord_2, double angle) {
    double coord_1_new = coord_1 * cos(angle) + coord_2 * sin(angle);
    double coord_2_new = -coord_1 * sin(angle) + coord_2 * cos(angle);
    coord_1 = coord_1_new;
    coord_2 = coord_2_new;
}

double Turn(const PointR3& p, const PointR3& q, const PointR3& r);

double Turn(const PointR3* p, const PointR3* q, const PointR3* r);

double Time(const PointR3& p, const PointR3& q, const PointR3& r);

double Time(const PointR3* p, const PointR3* q, const PointR3* r);

void HalfHull(std::vector<PointR3>& points, Hull& hull, bool is_lower);

std::vector<PointR3*> GetEvents(std::vector<PointR3>& points,
                                const size_t begin_idx, const size_t end_idx);

struct PointSet {
    void RotateAndAdd(double x, double y, double z, int id) {
        Rotate(z, y, eps);
        Rotate(x, z, eps);
        Rotate(x, y, eps);
        points.emplace_back(x, y, z, id);
    };
    void FlipZ() {
        for (auto& point : points) {
            point.z = -point.z;
            point.prev = nullptr;
            point.next = nullptr;
        }
    }
    void FillPointers();
    Hull MakeConvexHull();
    double AverageVoronoiPolygons();
    std::vector<PointR3> points;
    std::vector<size_t> degrees;
};

int main() {
    size_t points_count = 0;
    PointSet ps;
    while (true) {
        double x, y;
        std::cin >> x >> y;
        if(!std::cin.fail()) {
            ps.RotateAndAdd(x, y, x * x + y * y, points_count);
            ++points_count;
        } else {
            break;
        }
    }
    ps.degrees.resize(points_count);
    Hull hull = ps.MakeConvexHull();
    std::cout <<  std::setprecision(6) << std::fixed <<
              ps.AverageVoronoiPolygons() << std::endl;
    return 0;
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

void PointSet::FillPointers() {
    if (points.size() <= 1) {
        return;
    }
    points[0].next = &points[1];
    for (size_t i = 1; i < points.size() - 1; ++i) {
        points[i].prev = &points[i - 1];
        points[i].next = &points[i + 1];
    }
    if (points.size() >= 2) {
        points.back().prev = &points[points.size() - 2];
    }
}

Hull PointSet::MakeConvexHull() {
    std::stable_sort(points.begin(), points.end(),      // sorting points by x coordinate
                     [](const PointR3& first, const PointR3& second) {
                         return std::tie(first.x, first.y, first.z) <
                                std::tie(second.x, second.y, second.z); });
    //FillPointers();
    Hull res_hull;
    HalfHull(points, res_hull, true);
    for (auto& facet : res_hull.facets) {
        facet.visited = true;
        ++degrees[facet.x];
        ++degrees[facet.y];
        ++degrees[facet.z];
    }
    FlipZ();
    HalfHull(points, res_hull, false);
    for (auto& facet : res_hull.facets) {
        if (facet.visited) {
            continue;
        }
        facet.visited = true;
        degrees[facet.x] = 0;
        degrees[facet.y] = 0;
        degrees[facet.z] = 0;
    }
    return res_hull;
}

double PointSet::AverageVoronoiPolygons() {
    double sum = 0;
    size_t points_count = 0;
    for (auto& point : points) {
        if (degrees[point.id]) {
            ++points_count;
            sum += degrees[point.id];
        }
    }
    if (!points_count) {
        return 0;
    }
    return sum / points_count;
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
            if (event_time[i] > cur_time && event_time[i] < new_time) {
                new_time = event_time[i];
                new_time_idx = i;
            }
        }

        if (new_time_idx == EVENTS + 1) {
            break;
        }

        switch (new_time_idx) {
            case 0:
                if (left_point->x < u->x) {
                    events.push_back(left_point);
                }
                left_point->Act();
                event_1++;
                break;
            case 1:
                if (right_point->x > v->x) {
                    events.push_back(right_point);
                }
                right_point->Act();
                event_2++;
                break;
            case 2:
                events.push_back(v);
                v = v->next;
                break;
            case 3:
                v = v->prev;
                events.push_back(v);
                break;
            case 4:
                events.push_back(u);
                u = u->prev;
                break;
            case 5:
                u = u->next;
                events.push_back(u);
                break;
            default:
                std::cout << "How did you manage to get here?" << std::endl;
                break;
        }
        cur_time = new_time;
    }
    u->next = v;
    v->prev = u;

    // Updating pointers by reverse traverse
    for (auto i = static_cast<int64_t>(events.size() - 1); i >= 0; --i) {
        PointR3* event = events[i];
        if (event->x > u->x && event->x < v->x) {
            u->next = event;
            v->prev = event;
            event->prev = u;
            event->next = v;
            if (event->x <= points[middle_idx - 1].x) {
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