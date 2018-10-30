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
    friend std::ostream &operator<<(std::ostream &o, Point3D p) {
        o << '(' << p.x << ',' << p.y << ',' << p.z << ')';
        return o;
    }
};

public:
    friend class Draw;

    std::vector<Point3D> worldPoint;
    std::vector<Point3D> devicePoint;
    std::vector<Line> line;

    void rotate(float degree, Point3D &p1, Point3D &p2);

    void scale(float factor);

    void translate(Point3D &p);


};



#endif //SIMPLEDRAWINGSYSTEM_POLYHEDRON_H
