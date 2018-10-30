//
//  Draw.hpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/9/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

#ifndef Draw_hpp
#define Draw_hpp

#include <cmath>
#include <stdio.h>
#include <map>

#include "polygon.hpp"
#include "polyhedron.hpp"

enum View {XY, XZ, YZ};

class Draw {
    unsigned int x, y; // These are the dimensions of the drawable window
    float delta_x, delta_y, delta_z, delta;
    float *PixelBuffer; // This is where we can draw the pixels
    int min;
    const char x_min = 0b000001;
    const char x_max = 0b000010;
    const char y_min = 0b000100;
    const char y_max = 0b001000;
    const char z_min = 0b010000;
    const char z_max = 0b100000;
    std::map<char, float> ViewBox;
    View view;


public:
    bool bresenhamAlgo;
    bool ThreeDimensional;


    std::vector<Polygon> polygons;
    std::vector<Polyhedron> polyhedrons;


    Draw() : x(640), y(480), bresenhamAlgo(false)  {
        PixelBuffer = new float[(x) * (y) * 3];
        min = std::min(x,y);
        ThreeDimensional = false;
        view = XY;
    }
    Draw(unsigned int _x, unsigned int _y, bool ba, std::vector<Polygon> poly) : x(_x), y(_y), bresenhamAlgo(ba){
        PixelBuffer = new float[(x+1) * (y+1) * 3];
        min = std::min(x,y);
        ThreeDimensional = false;
        view = XY;
    }

    Polygon &operator[](int i){
        if(i < 0 || i >= polygons.size()) {
            std::cout << "ID " << i << " is not valid. Returning polygon" << i % polygons.size() << "." << std::endl;
        }
        return polygons[i % polygons.size()];
    }


    void MakePix (const Point &a);
    void MakePix (const int &x, const int &y);


    void xd2xp(Point &p);

    void draw();
    void draw(Vertex &a);
    void draw(Polygon & p);
    void draw(Polyhedron &p);
    void draw(Vertex &a, Vertex &b);


    operator float*() { return PixelBuffer; }

    // Basic lines
    void horizontalLine(Point &a, Point &b);
    void verticalLine(Point &a, Point &b);
    void diagonalLinePositive(Point &a, Point &b);
    void diagonalLineNegative(Point &a, Point &b);


    // Bresenham functions
    void bresenham(Point &a, Point &b, float delta_x, float delta_y);
    void bresenhamLTPO(Point &a, Point &b, float &delta_x, float &delta_y); // Less than positve one (postive slope)
    void bresenhamGTPO(Point &a, Point &b, float &delta_x, float &delta_y); // Greater than positive one (postive slope)
    void bresenhamLTNO(Point &a, Point &b, float &delta_x, float &delta_y); // Less than negative one (negative slope)
    void bresenhamGTNO(Point &a, Point &b, float &delta_x, float &delta_y); // Greater than negative one (negative slope)

    // Digital Differential Analyzer
    void digitalDifferentialAnalyzer(const Point &start, const Point &end, float &delta_x, float &delta_y);
    void DDAGreater(const Point &start, const Point &end, const float &slope);
    void DDARegular(const Point &start, const Point &end, const float &slope);

    // Clipping alogirthm
    void CohenSutherland(Polygon &poly);
    void locate(Point &p);
    float Xintersection (const Point &start, const Point &end, const float &xLine);
    float Yintersection (const Point &start, const Point &end, const float &xLine);

    void initialize(std::string file, bool td) {
        ThreeDimensional = td;
        if (ThreeDimensional) initializePolyhedrons(file);
        else initializePolygons(file);
    }
    void initializePolygons(std::string file);
    void initializePolyhedrons(std::string filename);

    void save(std::string filename) {
        std::ofstream file(filename);

        file << polygons.size() << std::endl;

        for (auto poly: polygons) {

            file << std::endl << poly.size() << std::endl;
            for (auto point: poly) {
                file << point.xr << " " << point.yr << std::endl;
            }
        }
        file.close();
    }

    void info(int id) {
        std::cout << "    Showing info for polygon " << id << std::endl;
        std::cout << "      Number of vertices: " << polygons[id].size() << std::endl;
        for (auto point: polygons[id]) {
            std::cout << "        (" << point.xr << "," << point.yr << ")" << std::endl;
        }
    }

    void normalize();
};
#endif /* Draw_hpp */