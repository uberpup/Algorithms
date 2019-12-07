/*
 * Даны точки, никакие 3 из которых не лежат на одной прямой. Никакие 4 точки не лежат на одной окружности. Кроме того, все точки имеют различные x-координаты. Определите среднее число сторон в многоугольниках диаграммы Вороного этого множества точек. Считаются только конечные многоугольники.
 * Если все многоугольники неограниченны, ответ полагается равным 0.
 * Число точек n ≤ 100000. Алгоритм должен иметь асимптотику O(n log n)
 */

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <list>
#include <vector>
#include <tuple>
#include <iomanip>

const double rot_eps = 1e-7;
const double cmp_eps = 1e-9;
const double inf = std::numeric_limits<double>::infinity();
const int EVENTS = 6;

enum PositioningCases {
    POINT_LEFT_OF_u = 0,
    POINT_RIGHT_OF_v = 1,
    BRIDGE_ULEFT_V = 2,
    BRIDGE_URIGHT_V = 3,
    BRIDGE_U_VRIGHT = 4,
    BRIDGE_U_VLEFT = 5
};

struct PointR3 {
    double x;
    double y;
    double z;
};

struct LinkedPointR3 : PointR3 {
    LinkedPointR3() = default;
    LinkedPointR3(double x, double y, double z) : x(x), y(y), z(z),
                                                  prev(nullptr), next(nullptr) {};
    LinkedPointR3(double x, double y, double z, int id) : x(x), y(y), z(z), id(id),
                                                          prev(nullptr), next(nullptr) {};
    LinkedPointR3(double x, double y, double z, LinkedPointR3* prev, LinkedPointR3* next) : x(x),
            y(y), z(z), prev(prev), next(next) {};
    LinkedPointR3(double x, double y, double z, int id, LinkedPointR3* prev, LinkedPointR3* next) :
            x(x), y(y), z(z), id(id), prev(prev), next(next) {};

    double x;
    double y;
    double z;

    LinkedPointR3* prev;
    LinkedPointR3* next;

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

LinkedPointR3 null_point = {inf, inf, inf, nullptr, nullptr};
LinkedPointR3* null_event = &null_point;

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

double Turn(const LinkedPointR3& p, const LinkedPointR3& q, const LinkedPointR3& r);

double Turn(const LinkedPointR3* p, const LinkedPointR3* q, const LinkedPointR3* r);

double Time(const LinkedPointR3& p, const LinkedPointR3& q, const LinkedPointR3& r);

double Time(const LinkedPointR3* p, const LinkedPointR3* q, const LinkedPointR3* r);

void HalfHull(std::vector<LinkedPointR3>& points, Hull& hull, bool is_lower);

std::vector<LinkedPointR3*> GetEvents(std::vector<LinkedPointR3>& points,
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
    double AverageVoronoiPolygons();
    std::vector<LinkedPointR3> points;
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


void Rotate(double& coord_1, double& coord_2, const double angle) {
    double cos_ = cos(angle);
    double sin_ = sin(angle);
    double coord_1_new = coord_1 * cos_ + coord_2 * sin_;
    double coord_2_new = -coord_1 * sin_ + coord_2 * cos_;
    coord_1 = coord_1_new;
    coord_2 = coord_2_new;
}

double Turn(const LinkedPointR3& p, const LinkedPointR3& q, const LinkedPointR3& r) {     // Helps to determine whether turn is cw or ccw
    return (q.x - p.x) * (r.y - p.y) - (r.x - p.x) * (q.y - p.y);
}

double Turn(const LinkedPointR3* p, const LinkedPointR3* q, const LinkedPointR3* r) {
    if (p == null_event || q == null_event || r == null_event) {
        return 1;
    }
    if (p == nullptr || q == nullptr || r == nullptr) {
        return 1;
    }
    return Turn(*p, *q, *r);
}

double Time(const LinkedPointR3& p, const LinkedPointR3& q, const LinkedPointR3& r) {     // 'Time' of a turn
    return ((q.x - p.x) * (r.z - p.z) - (r.x - p.x) * (q.z - p.z)) /
           Turn(p, q, r);
}

double Time(const LinkedPointR3* p, const LinkedPointR3* q, const LinkedPointR3* r) {
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
                     [](const LinkedPointR3& first, const LinkedPointR3& second) {
                         return std::tie(first.x, first.y, first.z) <
                                std::tie(second.x, second.y, second.z); });
    Hull res_hull;
    HalfHull(points, res_hull, true);
    FlipZ();
    HalfHull(points, res_hull, false);
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

void HalfHull(std::vector<LinkedPointR3>& points, Hull& hull, bool is_lower) {
    if (points.size() == 0) {
        return;
    }
    if (points.size() == 1) {
        points[0].prev = &null_point;
        points[0].next = &null_point;
        return;
    }

    std::vector<LinkedPointR3*> events = GetEvents(points, 0, points.size());
    for (auto event : events) {     // Selecting facets out of events
        Facet current = { event->prev->id, event->id, event->next->id };
        if (is_lower == !event->Act()) {
            std::swap(current.x, current.y);
        }
        hull.facets.push_back(current);
    }
}

// Divide and conquer
std::vector<LinkedPointR3*> GetEvents(std::vector<LinkedPointR3>& points,
                                      const size_t begin_idx, const size_t end_idx) {
    if (end_idx - begin_idx == 1) {     // two points automatically form the bridge
        return {};
    }
    const size_t middle_idx = (begin_idx + end_idx) / 2;
    std::vector<LinkedPointR3*> events;
    std::vector<LinkedPointR3*> events_buff[2] = {
            GetEvents(points, begin_idx, middle_idx),
            GetEvents(points, middle_idx, end_idx)
    };
    // Taking middle points to merge
    LinkedPointR3* u = &points[middle_idx - 1];
    LinkedPointR3* v = &points[middle_idx];

    while (Turn(u, v, v->next) < 0 || Turn(u->prev, u, v) < 0) {    // Finding initial bridge
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

    while (true) {      // finding and going through all times with certain events for current u and v
        LinkedPointR3* left_point = &null_point;
        LinkedPointR3* right_point = &null_point;
        double event_time[EVENTS];

        if (event_1 < events_buff[0].size()) {
            left_point = events_buff[0][event_1];
            event_time[POINT_LEFT_OF_u] = Time(left_point->prev, left_point, left_point->next);
        } else {
            event_time[POINT_LEFT_OF_u] = inf;
        }

        if (event_2 < events_buff[1].size()) {
            right_point = events_buff[1][event_2];
            event_time[POINT_RIGHT_OF_v] = Time(right_point->prev, right_point, right_point->next);
        } else {
            event_time[POINT_RIGHT_OF_v] = inf;
        }

        event_time[BRIDGE_ULEFT_V] = Time(u, v, v->next);
        event_time[BRIDGE_URIGHT_V] = Time(u, v->prev, v);
        event_time[BRIDGE_U_VRIGHT] = Time(u->prev, u, v);
        event_time[BRIDGE_U_VLEFT] = Time(u, u->next, v);

        size_t new_time_idx = EVENTS + 1;
        double new_time = inf;
        for (int i = 0; i < EVENTS; i++) {      // Choosing the closest event
            if (event_time[i] - cur_time > cmp_eps &&
                new_time - event_time[i] > cmp_eps) {
                new_time = event_time[i];
                new_time_idx = i;
            }
        }

        if (new_time_idx == EVENTS + 1) {       // If nothing more to happen
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
            case BRIDGE_ULEFT_V:
                events.push_back(v);
                v = v->next;
                break;
            case BRIDGE_URIGHT_V:
                v = v->prev;
                events.push_back(v);
                break;
            case BRIDGE_U_VRIGHT:
                events.push_back(u);
                u = u->prev;
                break;
            case BRIDGE_U_VLEFT:
                u = u->next;
                events.push_back(u);
                break;
            default:
                assert(false);
                break;
        }
        cur_time = new_time;
    }
    // Merging hulls
    u->next = v;
    v->prev = u;

    // Updating pointers by reverse traverse
    for (auto i = static_cast<int64_t>(events.size() - 1); i >= 0; --i) {
        LinkedPointR3* event = events[i];
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