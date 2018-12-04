#ifndef SHADEDPOLYHEDRON
#define SHADEDPOLYHEDRON
#include "Point.hpp"

// #include "polyhedron.hpp"
#include "helpers.hpp"
#include "polygon.hpp"
#include <vector>
#include <iostream>

// enum View;

struct SPolygon {
    int v1, v2, v3; // Vertex 1, vertex 2 vertex 3
    Point3D normal; // For the normal vector;
    SPolygon(int v1_, int v2_, int v3_): v1(v1_), v2(v2_), v3(v3_), normal(0,0,0) {}
};


class ShadedPolyhedron{

public:

    std::vector<Point3D> points;
    std::vector<SPolygon> polygons;
    Point3D center;

    void sortPolygons(View proj);
    void calculateNormals();
    Point3D &calculateCenter();
    Polygon iToPoly(int i, View view);

    ShadedPolyhedron() {};
};

#endif //SHADEDPOLYHEDRON
