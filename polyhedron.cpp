//
//  Draw.hpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/29/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

#include "helpers.hpp"
#include "Draw.hpp"
#include <cassert>

void Polyhedron::rotate(float degree, Point3D p1, Point3D p2) {
    // Find C
    while (degree <= -180) degree += 360;
    while (degree > 180) degree -= 360;

    degree *= M_PI / 180.0;

    Point3D C{0, 0, 0};
    for (Point3D &p: worldPoint) {
        C.x += p.x;
        C.y += p.y;
        C.z += p.z;
    }

    C.x /= worldPoint.size();
    C.y /= worldPoint.size();
    C.z /= worldPoint.size();

    float len = sqrtf(powf(p1.x - p2.x, 2) + powf(p1.y - p2.y, 2) + powf(p1.z - p2.z, 2));
//    std::cout << " len is " << len << std::endl;
    float c = cosf(degree), s = sinf(degree);

    Point3D u{
            (p2.x - p1.x) / len,
            (p2.y - p1.y) / len,
            (p2.z - p1.z) / len
    };

//    std::cout << sqrtf(powf(u.x, 2) + powf(u.y, 2) + powf(u.z, 2)) << std::endl;
    assert(fabsf(sqrtf(powf(u.x, 2) + powf(u.y, 2) + powf(u.z, 2)) - 1) < .01);

    // Single matrix equation pulled from
    // https://en.wikipedia.org/wiki/Rotation_matrix#General_rotations

    float R[3][3] = {
            {niceround(c + powf(u.x, 2) * (1 - c)),    niceround(u.x * u.y * (1 - c) - u.z * s), niceround(
                    u.x * u.z * (1 - c) + u.y * s)},
            {niceround(u.y * u.x * (1 - c) + u.z * s), niceround(c + powf(u.y, 2) * (1 - c)),    niceround(
                    u.y * u.z * (1 - c) - u.x * s)},
            {niceround(u.z * u.x * (1 - c) - u.y * s), niceround(u.z * u.y * (1 - c) + u.x * s), niceround(
                    c + powf(u.z, 2) * (1 - c))}
    };

    std::cout << "Rotating.. " << std::endl;
    for (Point3D &p: worldPoint) {
        std::cout << p << " to ";

        p = {
                p.x - C.x,
                p.y - C.y,
                p.z - C.z
        };

        p = {
                niceround((p.x * R[0][0] + p.y * R[1][0] + p.z * R[2][0])),
                niceround((p.x * R[0][1] + p.y * R[1][1] + p.z * R[2][1])),
                niceround((p.x * R[0][2] + p.y * R[1][2] + p.z * R[2][2])),
        };

        p = {
                p.x + C.x,
                p.y + C.y,
                p.z + C.z
        };
        std::cout << p << std::endl;

    }
}

void Polyhedron::scale(float factor) {
    // Matrix multiplication simplified
    float x_avg = 0, y_avg = 0, z_avg;
    for (Point3D &point: worldPoint) {
        x_avg += point.x;
        y_avg += point.y;
        z_avg += point.z;
    }

    x_avg /= worldPoint.size();
    y_avg /= worldPoint.size();
    z_avg /= worldPoint.size();

    std::cout << " Scaling.. " << std::endl;
    for (auto &p: worldPoint) {
        std::cout << p << " to ";
        p.x = (p.x - x_avg) * factor + x_avg;
        p.y = (p.y - y_avg) * factor + y_avg;
        p.z = (p.z - z_avg) * factor + z_avg;
        std::cout << p << std::endl;
    }
}

void Polyhedron::translate(Polyhedron::Point3D p) {
    std::cout << "Translating .. " << std::endl;
    for (Point3D &point: worldPoint) {
        std::cout << p << " to ";
        point.x += p.x;
        point.y += p.y;
        point.z += p.z;
        std::cout << p << std::endl;

    }
}

void Draw::draw(Polyhedron &p) {
//    std::cout << "drawing" << std::endl;
    for (Line &line: p.line) {
        draw(p.worldPoint[line.first], p.worldPoint[line.second]);
    }
}

void Draw::normalize() {
    ViewBox[x_min] = std::numeric_limits<float>::max();
    ViewBox[y_min] = std::numeric_limits<float>::max();
    ViewBox[z_min] = std::numeric_limits<float>::max();

    ViewBox[x_max] = std::numeric_limits<float>::min();
    ViewBox[y_max] = std::numeric_limits<float>::min();
    ViewBox[z_max] = std::numeric_limits<float>::min();

    for (Polyhedron &poly: polyhedrons) {
        for (Polyhedron::Point3D &p: poly.worldPoint) {
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

void Draw::oblique(Polyhedron::Point3D a, Polyhedron::Point3D b) {
    Polyhedron::Point3D d{b.x - a.x, b.y - a.y, b.z - a.z};

    float len = sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2) + powf(b.z - a.z, 2));

    // d is the direction vector
    d.x /= len;
    d.y /= len;
    d.z /= len; // we will replace this value to be t.z



    for (Polyhedron &poly: polyhedrons) {
        for (Polyhedron::Point3D &p: poly.worldPoint) {
            Polyhedron::Point3D t = p;
            p.save();

            p.x = t.x + t.z * (-d.x / d.z);
            p.y = t.y + t.z * (-d.y / d.z);
            p.z = 0;

        }

    }
}


void Draw::viewCavalier() {
    view = CAVALIER;
    oblique({0, 0, 0}, {1, 1, 1});
}

void Draw::viewCabinet() {
    view = CABINET;
    oblique({0, 0, 0}, {1, 1, 3});
}

void Draw::viewXY() {
    view = XY;
}

void Draw::viewXZ() {
    view = XZ;
}

void Draw::viewYZ() {
    view = YZ;
}

void Draw::undoOblique() {
    if (view == CAVALIER || view == CABINET) {
        for (Polyhedron &poly: polyhedrons) {
            for (Polyhedron::Point3D &p: poly.worldPoint) {
                p.restore();
            }
        }
    }
    view = XY;
}
