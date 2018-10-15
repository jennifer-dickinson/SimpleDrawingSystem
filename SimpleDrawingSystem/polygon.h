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

//using Point = std::pair<size_t, size_t>;

class Point : private std::pair<float, float> {
    
public:
    
    union {
        float x;
        float first;
    };
    union {
        float y;
        float second;
    };
    float xd, yd;
    int xp, yp;

    Point(): first(0), second(0), xd(0), yd(0), xp(0), yp(0) {};
    Point(float _x, float _y) : first(_x), second(_y), xd(_x), yd(_y), xp(_x), yp(_y) { };
//    Point(const Point &a) : first(a.first), second(a.second), xd(a.xd), yd(a.yd), xp(a.xp), yp(a.yp){    }

    friend std::ostream &operator<<(std::ostream &os, const Point &p);
};

class Polygon {

public:
    std::vector<Point> point;

    int vertices;

    Polygon() : vertices(0) {};

    Polygon(int v) : vertices(v) {};

    void addVertex(Point p);


};

std::vector<Polygon> initializePolygons(std::string file);

typedef Point Vertex;

#endif //ECS175PROJ1_POLYGON_H
