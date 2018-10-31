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

void Polyhedron::scale(float factor) {
}

void Draw::draw(Polyhedron &p) {
    std::cout << "drawing" << std::endl;
    if (view == XY) {
        for(auto line: p.line) {
            Vertex start = p.worldPoint[line.first].xy();
            Vertex end = p.worldPoint[line.second].xy();
            std::cout << start << end << std::endl;
            draw(start, end);
        }
    } else if (view == XZ) {
        for(auto line: p.line) {
            Vertex start = p.worldPoint[line.first].xz();
            Vertex end = p.worldPoint[line.second].xz();
            std::cout << start << end << std::endl;
            draw(start, end);
        }
    } else {
        for(auto line: p.line) {
            Vertex start = p.worldPoint[line.first].yz();
            Vertex end = p.worldPoint[line.second].yz();
            std::cout << start << end << std::endl;
            draw(start, end);
        }
    }

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
        std::istringstream nV(line);
        nV >> numVertices;

        // Initialize the temporary polyhedron
        Polyhedron temp;

        // Collect the vertices in the xyz space
        for (int j = 0; j < numVertices; j++) {
            getinput(input, line);

            std::istringstream v(line);
            float x, y, z;

            v >> x >> y >> z;
            Polyhedron::Point3D temppoint(x, y, z);
            temp.worldPoint.push_back(temppoint);

            std::cout << "    Adding point" << temppoint << std::endl;

        }

        // Get the number of lines
        getinput(input, line);

        int numLines;
        std::istringstream nl(line);
        nl >> numLines;

        // Collect the lines
        for (int j = 0; j < numLines; j++) {
            getinput(input, line);

            std::istringstream l(line);
            int start, end;

            l >> start >> end; // Note that the points are indexed starting at 1
            temp.line.push_back(Line(start - 1, end - 1));
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
