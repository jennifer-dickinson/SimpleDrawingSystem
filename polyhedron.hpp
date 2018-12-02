//
//  Draw.hpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/29/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

#ifndef SIMPLEDRAWINGSYSTEM_POLYHEDRON_H
#define SIMPLEDRAWINGSYSTEM_POLYHEDRON_H


#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <iomanip>

#include "Draw.hpp"


#define CABINET_DEGREE       67.5f
#define CAVALIER_DEGREE      45.0f

#define Line std::pair<unsigned int, unsigned int>

class Polyhedron {

public:
struct Point3D{
    float x, y, z, xold, yold, zold, r, g, b;

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
};

    friend class Draw;

    std::vector<Point3D> worldPoint;
    std::vector<Line> line;

    void rotate(float degree, Point3D p1, Point3D p2);

    void scale(float factor);

    void translate(Point3D p);

    void oblique(Point3D &d);


};



#endif //SIMPLEDRAWINGSYSTEM_POLYHEDRON_H
