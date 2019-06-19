#include "shadedPolyhedron.hpp"
#include "Draw.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <cmath>
#include <cassert>

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
            temp.points.push_back(tempPoint);

            std::cout << "   Adding point " << tempPoint;
            printf(" with RGB Color %.2f %.2f %.2f\n", temp.points.back().r, temp.points.back().g, temp.points.back().b);
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

            temp.polygons.push_back(SPolygon(v1, v2, v3));
            std::cout << "   Making a polygon from points ";
            std::cout << temp.points[v1] << " ";
            std::cout << temp.points[v2] << " ";
            std::cout << temp.points[v3] << std::endl;
        }
        temp.calculateNormals();
        sPolyhedrons.push_back(temp);
    }
    normalizeShader();
    sortShadedPolyhedrons();
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
        Point3D a(
            points[poly.v2].x - points[poly.v1].x,
            points[poly.v2].y - points[poly.v1].y,
            points[poly.v2].z - points[poly.v1].z
        );
        Point3D b(
            points[poly.v3].x - points[poly.v1].x,
            points[poly.v3].y - points[poly.v1].y,
            points[poly.v3].z - points[poly.v1].z
        );
        poly.normal = Point3D(
            a.y*b.z - a.z*b.y,
            -(a.x*b.z-a.z*b.x),
            a.x*b.y - a.y *b.x
        );
        float length = sqrtf( powf(poly.normal.x, 2) + powf(poly.normal.y, 2) + powf(poly.normal.z, 2));
        poly.normal = Point3D(
            poly.normal.x / length,
            poly.normal.y / length,
            poly.normal.z / length
        );
    }
}

void ShadedPolyhedron::sortPolygons(View proj) {
    // Part of painters algorithm. Sorts the polygons within a polyhedron.
    std::vector<SPolygon> sortedPolys;
    for(auto &poly: polygons) {
        // Get the center of the polygon in terms of the view projection.
        switch(proj) {
            case XY:
                poly.center = (points[poly.v1].z  + points[poly.v2].z + points[poly.v3].z) / 3;
                break;
            case YZ:
                poly.center = (points[poly.v1].x  + points[poly.v2].x + points[poly.v3].x) / 3;
                break;
            default:
                poly.center = (points[poly.v1].y  + points[poly.v2].y + points[poly.v3].y) / 3;
        }
        sortedPolys.push_back(poly);
    }
    std::sort(sortedPolys.begin(), sortedPolys.end(), std::greater<SPolygon>());
    polygons = sortedPolys;
}

Polygon ShadedPolyhedron::iToPoly(int i, View view) {
    Polygon temp;
    Point3D &p1 = points[polygons[i].v1];
    Point3D &p2 = points[polygons[i].v2];
    Point3D &p3 = points[polygons[i].v3];
    switch(view) {
        case XY:
        {
            temp.addVertex(p1.xy());
            temp.addVertex(p2.xy());
            temp.addVertex(p3.xy());
        }
        break;
        case YZ:
        {
            temp.addVertex(p1.yz());
            temp.addVertex(p2.yz());
            temp.addVertex(p3.yz());
        }
        break;
        default:
        {
            temp.addVertex(p1.xz());
            temp.addVertex(p2.xz());
            temp.addVertex(p3.xz());
        }
    }

    temp[0].r = p1.r;
    temp[0].g = p1.g;
    temp[0].b = p1.b;

    temp[1].r = p2.r;
    temp[1].g = p2.g;
    temp[1].b = p2.b;

    temp[2].r = p3.r;
    temp[2].g = p3.g;
    temp[2].b = p3.b;

    // std::cout << "Generated polygon with points " << temp[0] << temp[1] << temp[2] <<std::endl;;
    return temp;
}

void Draw::draw(ShadedPolyhedron& p) {
    // draw the lines firstTime
    sortShadedPolyhedrons();
    for(int i = 0; i < p.polygons.size(); i++) {
        // rasterize the Polygon
        Polygon temp = p.iToPoly(i,view); // get the 2 dimensional view of the polygon
        rasterize(temp);
    }
}

void Draw::sortShadedPolyhedrons() {

    /* Painter's Algorithm */

    // First we sort the polyhedron's polygons
    for(ShadedPolyhedron &p: sPolyhedrons) {
        Point3D point = p.points[0];
        p.sortPolygons(view);
    }

    // Then we sort the polyhedrons.
    std::sort(sPolyhedrons.begin(), sPolyhedrons.end(), std::greater<ShadedPolyhedron>());
}
