//
//  Draw.hpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/29/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

#include "Draw.hpp"

void getinput(std::ifstream &input, std::string &line) {
    do {
        std::getline(input, line);
    } while (line.empty());
}

void Draw::draw(Polyhedron &p) {

}

void Draw::normalize() {
    ViewBox[x_min] = std::numeric_limits<float>::max();
    ViewBox[y_min] = std::numeric_limits<float>::max();
    ViewBox[z_min] = std::numeric_limits<float>::max();

    ViewBox[x_max] = std::numeric_limits<float>::min();
    ViewBox[y_max] = std::numeric_limits<float>::min();
    ViewBox[z_max] = std::numeric_limits<float>::min();

    for(Polyhedron &poly: polyhedrons) {
        for(Polyhedron::Point3D &p: poly.worldPoint) {
            if (p.x > ViewBox[x_max]) ViewBox[x_max] = p.x;
            if (p.x < ViewBox[x_min]) ViewBox[x_min] = p.x;
            if (p.y > ViewBox[y_max]) ViewBox[y_max] = p.y;
            if (p.y < ViewBox[y_min]) ViewBox[y_min] = p.y;
            if (p.z > ViewBox[z_max]) ViewBox[z_max] = p.z;
            if (p.z < ViewBox[z_min]) ViewBox[z_min] = p.z;
        }
    }

    delta_x = ViewBox[x_max] - ViewBox[x_min];
    delta_y = ViewBox[y_max] - ViewBox[y_min];
    delta_z = ViewBox[z_max] - ViewBox[z_min];

    delta = std::max(delta_x, delta_y);
    delta = std::max(delta, delta_z);
}

void Draw::initializePolyhedrons(std::string filename) {
    std::ifstream input(filename);

    if (!input) {
        std::cout << "Could not find file: " << filename << std::endl;
        std::exit(0);
    }

    std::string line;
    std::getline(input, line);
    std::istringstream ss(line);

    // Get the number of polyhedrons
    unsigned int numPolyhedrons;
    ss >> numPolyhedrons;

    for (int i = 0; i < numPolyhedrons; i++) {
        std::cout << "Adding polyhedron #" << i << std::endl;

        // Get the number of vertices
        getinput(input, line);

        unsigned int numVertices;
        ss = std::istringstream(line);
        ss >> numVertices;

        // Initialize the temporary polyhedron
        Polyhedron temp;

        // Collect the vertices in the xyz space
        for (int j = 0; j < numVertices; j++) {
            getinput(input, line);

            ss = std::istringstream(line);
            float x, y, z;

            ss >> x >> y >> z;
            Polyhedron::Point3D temppoint{x, y, z};
            temp.worldPoint.emplace_back(temppoint);

            std::cout << "    Adding point" << temppoint << std::endl;

        }

        // Get the number of lines
        getinput(input, line);

        int numLines;
        ss = std::istringstream(line);
        ss >> numLines;

        // Collect the lines
        for (int j = 0; j < numLines; j++) {
            getinput(input, line);

            ss = std::istringstream(line);
            int start, end;

            ss >> start >> end; // Note that the points are indexed starting at 1
            temp.line.emplace_back(start - 1, end - 1);
            std::cout << "  Making line from point " << start << " to " << end << std::endl;
        }

        // Push the completed polyhedrons onto the stack
        polyhedrons.push_back(temp);
    }

    // Normalize the viewbox
    normalize();

//    for (Polyhedron &poly: polyhedrons) {
//
//        for (Polyhedron::Point3D &p: poly.worldPoint) std::cout << p << std::endl;
//        for (Line &l: poly.line) std::cout << "Line: " << l.first << " " << l.second << std::endl;
//    }
//
//    std::cout << ViewBox[x_min] << " " << ViewBox[x_max] <<std::endl;
//    std::cout << ViewBox[y_min] << " " << ViewBox[y_max] <<std::endl;
//    std::cout << ViewBox[z_min] << " " << ViewBox[z_max] <<std::endl;

}
