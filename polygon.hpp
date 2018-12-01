//
// Created by Jennifer Salas on 10/2/18.
//

#ifndef ECS175PROJ1_POLYGON_H
#define ECS175PROJ1_POLYGON_H

#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <limits>
#include <cmath>


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
    Point(float _x, float _y) : first(_x), second(_y), xd(_x), yd(_y), xp(_x), yp(_y), xr(_x), yr(_y), region(0) r(1), g(1), b(1) { };

    bool operator<(const Point &p) {
        return x <= p.x && y <= p.y;
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &p);
};

class Polygon: public std::vector<Point> {
public:

    Polygon() {};

    void addVertex(Point p);

    void scale(const float &x, const float &y);

    void translate(const float &x, const float &y);

    void rotate(const float &deg);

    Point &operator[](int d) {
        if (d < 0)  d = (int)size() - (abs(d % (int)size()));
        return std::vector<Point>::operator[](d % size());
    }

};

typedef Point Vertex;

#endif //ECS175PROJ1_POLYGON_H
