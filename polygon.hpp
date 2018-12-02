//
// Created by Jennifer Salas on 10/2/18.
//

#ifndef ECS175PROJ1_POLYGON_H
#define ECS175PROJ1_POLYGON_H

#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include "Point.hpp"

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


#endif //ECS175PROJ1_POLYGON_H
