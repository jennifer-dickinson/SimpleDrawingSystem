#include "shadedPolyhedron.hpp"
#include "Draw.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <cmath>

void Draw::initializeShapes(std::string filename) {
    /*
        Load the file
    */
    std::ifstream input(filename);

    if (!input) {
        std::cout << "Could not find file: " << filename << std::endl;
        std::exit(0);
    }

    // Variables which will be used extensively
    std::string line;
    std::getline(input, line);
    std::istringstream ss(line);

    unsigned int numPolyhedrons;
    ss >> numPolyhedrons;

    for (int i = 0; i < numPolyhedrons; i++) {
        std::cout << "Adding polyhedron #" << i << std::endl;

        getinput(input, line);

        unsigned int numVertices;
        std::istringstream nV(line);
        nV >> numVertices;

        ShadedPolyhedron temp;

        for (int j = 0; j < numVertices; j++) {
            getinput(input, line);

            std::istringstream v(line);
            float x, y, z, r, g, b;

            v >> x >> y >> z >> r >> g >> b;
            Point3D tempPoint(x, y, z);
            tempPoint.setColor(r, g, b);
            temp.points.emplace_back(x,y,z);

            std::cout << "   Adding point " << tempPoint;
            printf(" with RGB Color %.2f %.2f %.2f\n", r, g, b);
        }

        // Get the number of triangles in the shape
        getinput(input, line);

        int numTriangles;
        std::istringstream nt(line);
        nt >> numTriangles;

        for (int j = 0; j < numTriangles; j++) {
            getinput(input, line);

            std::istringstream t(line);
            int v1, v2, v3;
            t >> v1 >> v2 >> v3;

            temp.polygons.emplace_back(v1, v2, v3);
            std::cout << "   Making a polygon from points ";
            std::cout << temp.points[v1] << " ";
            std::cout << temp.points[v2] << " ";
            std::cout << temp.points[v3] << std::endl;
        }
        temp.calculateNormals();
        sPolyhedrons.push_back(temp);
    }
    normalizeShader();
    std::cout << "Complete" <<std::endl;
}

Point3D &ShadedPolyhedron::calculateCenter()  {
    for(Point3D &point: points) {
        center.x += point.x;
        center.y += point.y;
        center.z += point.z;
    }
    center.x /= points.size();
    center.y /= points.size();
    center.z /= points.size();
    return center;
}

void Draw::normalizeShader() {
    ViewBox[x_min] = std::numeric_limits<float>::max();
    ViewBox[y_min] = std::numeric_limits<float>::max();
    ViewBox[z_min] = std::numeric_limits<float>::max();

    ViewBox[x_max] = std::numeric_limits<float>::min();
    ViewBox[y_max] = std::numeric_limits<float>::min();
    ViewBox[z_max] = std::numeric_limits<float>::min();

    for (ShadedPolyhedron &poly: sPolyhedrons) {
        for (Point3D &p: poly.points) {
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
   std::cout << "X boundaries: " << ViewBox[x_max] << " " << ViewBox[x_min] << std::endl;
   std::cout << "Y boundaries: " << ViewBox[y_max] << " " << ViewBox[y_min] << std::endl;
   std::cout << "Z boundaries: " << ViewBox[z_max] << " " << ViewBox[z_min] << std::endl;

}

void ShadedPolyhedron::calculateNormals() {
    /*
        Find the normals of each face of the polyhedron;
    */
    // calculateCenter();
    for (SPolygon &poly: polygons) {
        Point3D a{
            points[poly.v2].x - points[poly.v1].x,
            points[poly.v2].y - points[poly.v1].y,
            points[poly.v2].z - points[poly.v1].z
        };
        Point3D b{
            points[poly.v3].x - points[poly.v1].x,
            points[poly.v3].y - points[poly.v1].y,
            points[poly.v3].z - points[poly.v1].z
        };
        poly.normal = {
            a.y*b.z - a.z*b.y,
            -(a.x*b.z-a.z*b.x),
            a.x*b.y - a.y *b.x
        };
        float length = sqrtf( powf(poly.normal.x, 2) + powf(poly.normal.y, 2) + powf(poly.normal.z, 2));
        poly.normal = {
            poly.normal.x / length,
            poly.normal.y / length,
            poly.normal.z / length
        };
        std::cout << "Normal is " << poly.normal << std::endl;
    }
}

void Draw::draw(ShadedPolyhedron& p) {
    // draw the lines firstTime
    for(SPolygon &polygon: p.polygons) {
        draw(p.points[polygon.v1], p.points[polygon.v2]);
        draw(p.points[polygon.v2], p.points[polygon.v3]);
        draw(p.points[polygon.v3], p.points[polygon.v1]);

        std::cout << "   Drawing a polygon from points ";
        std::cout << p.points[polygon.v1] << " ";
        std::cout << p.points[polygon.v2] << " ";
        std::cout << p.points[polygon.v3] << std::endl;
    }
    std::cout << "Finished drawing polygon" << std::endl;
}
