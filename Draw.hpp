//
//  Draw.hpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/9/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

#ifndef Draw_hpp
#define Draw_hpp

#include <algorithm>
#include <cmath>
#include <stdio.h>
#include <map>

#include "polygon.hpp"
#include "polyhedron.hpp"
#include "shadedPolyhedron.hpp"
#include "Point.hpp"
#include "helpers.hpp"
#include "button.hpp"

class Draw {
    unsigned int x, y; // These are the dimensions of the drawable window
    float delta_x, delta_y, delta_z, delta;
    float *PixelBuffer; // This is where we can draw the pixels
    int min;
    const char x_min = 0b000001;
    const char y_max = 0b000010;
    const char x_max = 0b000100;
    const char y_min = 0b001000;
    const char z_min = 0b010000;
    const char z_max = 0b100000;
    std::map<char, float> ViewBox;
    View view;

public:
    bool bresenhamAlgo;
    bool ThreeDimensional;
    bool Shader;
    bool raster;


    std::vector<Polygon> polygons;
    std::vector<Polyhedron> polyhedrons;
    std::vector<ShadedPolyhedron> sPolyhedrons;
    std::vector<Button> buttons;


    Draw() : x(640), y(480), bresenhamAlgo(false)  {
        PixelBuffer = new float[(x) * (y) * 3];
        min = std::min(x,y);
        ThreeDimensional = false;
        Shader = false;
        view = XY;
        raster = false;
    }
    Draw(unsigned int _x, unsigned int _y, bool ba) : x(_x), y(_y), bresenhamAlgo(ba){
        PixelBuffer = new float[(x+1) * (y+1) * 3];
        min = std::min(x,y);
        ThreeDimensional = false;
        Shader = false;
        view = XY;
        raster = false;
    }

    Polygon &operator[](int i){
        if(i < 0 || i >= polygons.size()) {
            std::cout << "ID " << i << " is not valid. Returning polygon" << i % polygons.size() << "." << std::endl;
        }
        return polygons[i % polygons.size()];
    }

    void viewXY();
    void viewXZ();
    void viewYZ();
    void viewCavalier();
    void viewCabinet ();
    void ObliqueProjection(float degree);
    void undoOblique();


    void MakePix (const Point &a);
    void MakePix (const int &x, const int &y);
    void MakePix (const int &x, const int &y, float r, float g, float b);


    void xd2xp(Point &p);

    void draw();
    void draw(Vertex &a);
    void draw(Polygon & p);
    void draw(Polyhedron &p);
    void draw(ShadedPolyhedron &p);
    void draw(Vertex &a, Vertex &b);
    void draw(Point3D &a_, Point3D &b_);
    void oblique(Point3D a, Point3D b);


        operator float*() { return PixelBuffer; }

    // Basic lines
    void horizontalLine(Point &a, Point &b);
    void verticalLine(Point &a, Point &b);
    void diagonalLinePositive(Point &a, Point &b);
    void diagonalLineNegative(Point &a, Point &b);

    void rasterize(Polygon &c);


    // Bresenham functions
    void bresenham(Point &a, Point &b, float delta_x, float delta_y);
    void bresenhamLTPO(Point &a, Point &b, float &delta_x, float &delta_y); // Less than positive one (positive slope)
    void bresenhamGTPO(Point &a, Point &b, float &delta_x, float &delta_y); // Greater than positive one (positive slope)
    void bresenhamLTNO(Point &a, Point &b, float &delta_x, float &delta_y); // Less than negative one (negative slope)
    void bresenhamGTNO(Point &a, Point &b, float &delta_x, float &delta_y); // Greater than negative one (negative slope)

    // Digital Differential Analyzer
    void digitalDifferentialAnalyzer(const Point &start, const Point &end, float &delta_x, float &delta_y);
    void DDAGreater(const Point &start, const Point &end, const float &slope);
    void DDARegular(const Point &start, const Point &end, const float &slope);

    // Clipping alogirthm
    void CohenSutherland(Polygon &poly);
    void locate(Point &p);
    void ClippingPointAddOn(Polygon &poly, const int &p, const int &pnext, char OOBR);

    void initialize(std::string file, bool td, bool sh) {
        ThreeDimensional = td;
        Shader = sh;
        if (Shader) initializeShapes(file);
        else if (ThreeDimensional) initializePolyhedrons(file);
        else initializePolygons(file);
    }
    void initializePolygons(std::string file);
    void initializePolyhedrons(std::string filename);
    void initializeShapes(std::string filename);

    void save(std::string filename) {
        std::ofstream file(filename);

        file << polygons.size() << std::endl;

        for (Polygon &poly: polygons) {

            file << std::endl << poly.size() << std::endl;
            for (Point &point: poly) {
                file << point.xr << " " << point.yr << std::endl;
            }
        }
        file.close();
    }

    void info(int id) {
        int id_ = id % polygons.size();
        std::cout << "    Showing info for polygon " << id_ << std::endl;
        std::cout << "      Number of vertices: " << polygons[id_].size() << std::endl;
        for (Point &point: polygons[id_]) {
            std::cout << "        (" << point.xr << "," << point.yr << ")" << std::endl;
        }
    }

    void sortShadedPolyhedrons();

    void normalize();
    void normalizeShader();

    void save3D(std::string filename) {
        std::ofstream file(filename);

        file << polyhedrons.size() << std::endl;

        for(int i = 0; i < polyhedrons.size(); i++) {
            file << polyhedrons[i].worldPoint.size() << std::endl;
            for(Point3D &p: polyhedrons[i].worldPoint) {
                file<< p.x << " " << p.y << " " << p.z << std::endl;
            }
            file << polyhedrons[i].line.size() << std::endl;
            for (Point::pair<unsigned int, unsigned int> &pair: polyhedrons[i].line) {
                file << pair.first << " " << pair.second << std::endl;
            }
        }
        file.close();
    }
};
#endif /* Draw_hpp */
