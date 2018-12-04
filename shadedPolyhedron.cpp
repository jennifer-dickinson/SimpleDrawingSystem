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
    sortShadedPolyhedrons();
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
        // std::cout << "Normal is " << poly.normal << std::endl;
    }
}

void ShadedPolyhedron::sortPolygons(View proj) {
    // Part of painters algorithm. Sorts the polygons within a polyhedron.
    for(int i = 0; i < polygons.size(); i++) {
        for (int j = i + 1; j < polygons.size(); j++) {
            SPolygon &ith = polygons[i];
            SPolygon &jth = polygons[j];
            int imin, jmin;
            switch(proj) {
                case XY:
                {
                    std::vector<float> izs({points[ith.v1].z, points[ith.v2].z, points[ith.v3].z});
                    std::vector<float> jzs({points[jth.v1].z, points[jth.v2].z, points[jth.v3].z});
                    imin = *std::min_element(izs.begin(), izs.end());
                    jmin = *std::min_element(jzs.begin(), jzs.end());
                }
                    break;
                case YZ:
                {
                    std::vector<float> ixs({points[ith.v1].x, points[ith.v2].x, points[ith.v3].x});
                    std::vector<float> jxs({points[jth.v1].x, points[jth.v2].x, points[jth.v3].x});
                    imin = *std::min_element(ixs.begin(), ixs.end());
                    jmin = *std::min_element(jxs.begin(), jxs.end());
                }
                    break;
                default:
                {
                    std::vector<float> iys({points[ith.v1].y, points[ith.v2].y, points[ith.v3].y});
                    std::vector<float> jys({points[jth.v1].y, points[jth.v2].y, points[jth.v3].y});
                    imin = *std::min_element(iys.begin(), iys.end());
                    jmin = *std::min_element(jys.begin(), jys.end());
                }
            }
            if (imin > jmin)
                std::swap(polygons[i], polygons[j]);
        }
    }
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
    // sortShadedPolyhedrons();
    for(int i = 0; i < p.polygons.size(); i++) {
        // rasterize the Polygon

        Polygon temp = p.iToPoly(i,view);
        rasterize(temp);

        // Draw the skeleton;
        draw(p.points[p.polygons[i].v1], p.points[p.polygons[i].v2]);
        draw(p.points[p.polygons[i].v2], p.points[p.polygons[i].v3]);
        draw(p.points[p.polygons[i].v3], p.points[p.polygons[i].v1]);
    }
    std::cout << "Finished drawing polygon" << std::endl;
}

void Draw::sortShadedPolyhedrons() {

    /* Painter's Algorithm */

    // First we sort the polyhedron's polygons
    for(ShadedPolyhedron &p: sPolyhedrons) {
        Point3D point = p.points[0];
        assert(point.r == p.points[0].r);
        p.sortPolygons(view);
    }

    // Now we sort the polyhedrons according the view;
    for(int i = 0; i < sPolyhedrons.size(); i++) {
        for (int j = i + 1; j < sPolyhedrons.size(); j++) {
            ShadedPolyhedron &ith = sPolyhedrons[i];
            ShadedPolyhedron &jth = sPolyhedrons[j];
            int imin, jmin;
            switch(view) {
                case XY:
                {
                    std::vector<float> izs({ith.points[ith.polygons[0].v1].z, ith.points[ith.polygons[0].v2].z, ith.points[ith.polygons[0].v3].z});
                    std::vector<float> jzs({jth.points[jth.polygons[0].v1].z, jth.points[jth.polygons[0].v2].z, jth.points[jth.polygons[0].v3].z});
                    imin = *std::min_element(izs.begin(), izs.end());
                    jmin = *std::min_element(jzs.begin(), jzs.end());
                }
                    break;
                case YZ:
                {
                    std::vector<float> ixs({ith.points[ith.polygons[0].v1].x, ith.points[ith.polygons[0].v2].x, ith.points[ith.polygons[0].v3].x});
                    std::vector<float> jxs({jth.points[jth.polygons[0].v1].x, jth.points[jth.polygons[0].v2].x, jth.points[jth.polygons[0].v3].x});
                    imin = *std::min_element(ixs.begin(), ixs.end());
                    jmin = *std::min_element(jxs.begin(), jxs.end());
                }
                    break;
                default:
                {
                    std::vector<float> iys({ith.points[ith.polygons[0].v1].y, ith.points[ith.polygons[0].v2].y, ith.points[ith.polygons[0].v3].y});
                    std::vector<float> jys({jth.points[jth.polygons[0].v1].y, jth.points[jth.polygons[0].v2].y, jth.points[jth.polygons[0].v3].y});
                    imin = *std::min_element(iys.begin(), iys.end());
                    jmin = *std::min_element(jys.begin(), jys.end());
                }
            }
            if (imin > jmin)
                std::swap(sPolyhedrons[i], sPolyhedrons[j]);


        }
    }
}
