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
#include "Point.hpp"


#define CABINET_DEGREE       67.5f
#define CAVALIER_DEGREE      45.0f

#define Line std::pair<unsigned int, unsigned int>

class Polyhedron {

public:

    friend class Draw;

    std::vector<Point3D> worldPoint;
    std::vector<Line> line;

    void rotate(float degree, Point3D p1, Point3D p2);

    void scale(float factor);

    void translate(Point3D p);

    void oblique(Point3D &d);


};



#endif //SIMPLEDRAWINGSYSTEM_POLYHEDRON_H
