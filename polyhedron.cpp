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

void Polyhedron::rotate(float degree, Point3D p1, Point3D p2) {
    // Find C
    Point3D C{0,0,0};
    for(Point3D &p: worldPoint) {
        C.x += p.x;
        C.y += p.y;
        C.z += p.z;
    }
    // Translate by -C
    translate(-C);
    // Rotate
    // Trnslate by +C
    translate(C);
}

void Polyhedron::scale(float factor) {
    // Matrix multiplication simplified
    float x_avg = 0, y_avg = 0, z_avg;
    for(Point3D &point: worldPoint) {
        x_avg += point.x;
        y_avg += point.y;
        z_avg += point.z;
    }

    x_avg /= worldPoint.size();
    y_avg /= worldPoint.size();
    z_avg /= worldPoint.size();

    std::cout << " Scaling.. " << std::endl;
    for (auto &p: worldPoint) {
        std::cout << "    from (" << p.x << "," << p.y << ") to ";
        p.x = (p.x - x_avg) * factor + x_avg;
        p.y = (p.y - y_avg) * factor + y_avg;
        p.z = (p.z - z_avg) * factor + z_avg;
        std::cout << "(" << p.x << "," << p.y << "," << p.z << ")" << std::endl;
    }
}
void Polyhedron::translate(Polyhedron::Point3D p) {
    for(Point3D &point: worldPoint) {
        point.x += p.x;
        point.y += p.y;
        point.z += p.z;
    }
}

void Draw::draw(Polyhedron &p) {
//    std::cout << "drawing" << std::endl;
    for(Line &line: p.line) {
        Vertex start, end;
        if (view == XY) {
            start = p.worldPoint[line.first].xy();
            end = p.worldPoint[line.second].xy();
        } else if (view == XZ) {
            start = p.worldPoint[line.first].xz();
            end = p.worldPoint[line.second].xz();
        } else {
            start = p.worldPoint[line.first].yz();
            end = p.worldPoint[line.second].yz();
        }
        std::cout << start << end << std::endl;
        draw(start, end);
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

    return;
//    std::cout << "X boundaries: " << ViewBox[x_max] << " " << ViewBox[x_min] << std::endl;
//    std::cout << "Y boundaries: " << ViewBox[y_max] << " " << ViewBox[y_min] << std::endl;
//    std::cout << "Z boundaries: " << ViewBox[z_max] << " " << ViewBox[z_min] << std::endl;

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
