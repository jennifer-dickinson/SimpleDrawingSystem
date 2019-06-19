#ifndef SHADEDPOLYHEDRON
#define SHADEDPOLYHEDRON
#include "Point.hpp"

// #include "polyhedron.hpp"
#include "helpers.hpp"
#include "polygon.hpp"
#include <vector>
#include <iostream>
#include <algorithm>

// enum View;

struct SPolygon {
    int v1, v2, v3; // Vertex 1, vertex 2 vertex 3
    Point3D normal; // For the normal vector;
    SPolygon(int v1_, int v2_, int v3_): v1(v1_), v2(v2_), v3(v3_), normal(0,0,0), center(0) {}
    float center;
    friend bool operator<(const SPolygon&  cmp, const SPolygon& cmp2) {
        return cmp.center < cmp2.center;
    }
    friend bool operator>(const SPolygon&  cmp, const SPolygon& cmp2) {
        return cmp.center > cmp2.center;
    }
};

class ShadedPolyhedron{

public:

    std::vector<Point3D> points;
    std::vector<SPolygon> polygons;
    Point3D center;
    float viewCenter;

    void sortPolygons(View proj);
    void calculateNormals();
    Point3D &calculateCenter();
    Polygon iToPoly(int i, View view);

    ShadedPolyhedron() {};

    friend bool operator<(const ShadedPolyhedron&  cmp, const ShadedPolyhedron& cmp2) {
        return cmp.viewCenter < cmp2.viewCenter;
    }
    friend bool operator>(const ShadedPolyhedron&  cmp, const ShadedPolyhedron& cmp2) {
        return cmp.viewCenter > cmp2.viewCenter;
    }
};

#endif //SHADEDPOLYHEDRON
