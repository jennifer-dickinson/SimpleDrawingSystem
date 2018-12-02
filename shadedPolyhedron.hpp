#ifndef SHADEDPOLYHEDRON
#define SHADEDPOLYHEDRON
#include "Point.hpp"

// #include "polyhedron.hpp"
#include "helpers.hpp"
#include <vector>
#include <iostream>

struct SPolygon {
    int v1, v2, v3; // Vertex 1, vertex 2 vertex 3
    float nx, ny, nz; // For the normal vector;
    SPolygon(int v1_, int v2_, int v3_): v1(v1_), v2(v2_), v3(v3_), nx(0), ny(0), nz(0) {}
};


class ShadedPolyhedron{

public:

    std::vector<Point3D> points;
    std::vector<SPolygon> polygons;
    Point3D center;

    void calculateNormals();
    Point3D &calculateCenter();

    ShadedPolyhedron() {};
};

#endif //SHADEDPOLYHEDRON
