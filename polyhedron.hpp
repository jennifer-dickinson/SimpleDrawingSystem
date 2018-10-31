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

#include "Draw.hpp"


#define Line std::pair<unsigned int, unsigned int>

class Polyhedron {

struct Point3D{
    float x, y, z;

    Point3D(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    friend std::ostream &operator<<(std::ostream &o, Point3D p) {
        o << '(' << p.x << ',' << p.y << ',' << p.z << ')';
        return o;
    }
    Vertex xy() { return Vertex(x,y); }
    Vertex xz() { return Vertex(x,z); }
    Vertex yz() { return Vertex(y,z); }
};

public:
    friend class Draw;

    std::vector<Point3D> worldPoint;
    std::vector<Line> line;

    void rotate(float degree, Point3D &p1, Point3D &p2);

    void scale(float factor);

    void translate(Point3D &p);


};



#endif //SIMPLEDRAWINGSYSTEM_POLYHEDRON_H
