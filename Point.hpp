#ifndef POINT
#define POINT
#include <iostream>
#include <iomanip>

class Point : public std::pair<float, float> {

public:

    union {
        float x;
        float first;
    };
    union {
        float y;
        float second;
    };

    float xr, yr; // real coordinates
    float xd, yd; // device coordinates
    int xp, yp;   // pixel coordinates
    char region;

    float r,g,b;

    Point(): first(0), second(0), xd(0), yd(0), xp(0), yp(0), xr(0), yr(0), region(0), r(1), g(1), b(1) {};
    Point(float _x, float _y) : first(_x), second(_y), xd(_x), yd(_y), xp(_x), yp(_y), xr(_x), yr(_y), region(0), r(1), g(1), b(1) { };

    Point T() {
        return Point(yr, xr);
    }

    bool operator<(const Point &p) {
        return x <= p.x && y <= p.y;
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &p);
};
typedef Point Vertex;

struct Point3D{
    float x, y, z, xold, yold, zold, r, g, b;
    Point3D() {};
    Point3D(float x_, float y_, float z_) : x(x_), y(y_), z(z_), r(1), g(1), b(1) {}
    friend std::ostream &operator<<(std::ostream &o, Point3D p) {
        o << std::setprecision(1) << '(' << p.x << ',' << p.y << ',' << p.z << ')';
        return o;
    }
    Point3D operator-() {
        return Point3D(-x, -y, -z);
    }
    Vertex xy() { return Vertex(x, y); }
    Vertex xz() { return Vertex(x, z); }
    Vertex yz() { return Vertex(y, z); }

    void save() {xold = x; yold = y; zold = z;}
    void restore() {x = xold; y = yold; z = zold;}

    void setColor(float r_, float g_, float b_) { r = r_; g = g_; b = b_;}

};
#endif
