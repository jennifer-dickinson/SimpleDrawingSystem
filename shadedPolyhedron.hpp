#ifndef SHADEDPOLYHEDRON
#define SHADEDPOLYHEDRON
#include "Point.hpp"

// #include "polyhedron.hpp"
#include "helpers.hpp"
#include <vector>
#include <iostream>

struct Point3D{
    float x, y, z, xold, yold, zold, r, g, b;
    Point3D() {};
    Point3D(float x_, float y_, float z_) : x(x_), y(y_), z(z_), r(1), g(1), b(1) {}
    friend std::ostream &operator<<(std::ostream &o, Point3D p) {
        o << std::setprecision(1) << '(' << p.x << ',' << p.y << ',' << p.z << ')';
        return o;
    }
    Point3D operator-() {
        return {-x, -y, -z};
    }
    Vertex xy() { return {x, y}; }
    Vertex xz() { return {x, z}; }
    Vertex yz() { return {y, z}; }

    void save() {xold = x; yold = y; zold = z;}
    void restore() {x = xold; y = yold; z = zold;}

    void setColor(float r_, float g_, float b_) { r = r_; g = g_; b = b_;}

};

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
