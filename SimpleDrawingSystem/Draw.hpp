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

#include "polygon.hpp"


class Draw {
    unsigned int x;
    unsigned int y;
    float *PixelBuffer;
    int min;
    
    bool bresenhamAlgo;
    
public:
    
    std::vector<Polygon> polygons;

    
    Draw() : x(640), y(480), bresenhamAlgo(true)  {
        PixelBuffer = new float[(x+1) * (y+1) * 3];
        min = std::min(x,y);
    }
    Draw(unsigned int _x, unsigned int _y, bool ba, std::vector<Polygon> poly) : x(_x), y(_y), bresenhamAlgo(ba){
        PixelBuffer = new float[(x+1) * (y+1) * 3];
        min = std::min(x,y);
    }
    
    void setWorld(std::vector<Polygon> &poly) {
        polygons = poly;
    }
    
    void digitalDifferenceAnalyzer(Point p1, Point p2, int delta_x, int delta_y);
    
    void MakePix (const Point &a);
    void MakePix (const int &x, const int &y);
    
    
    void xd2xp(Point &p);
    
    void draw(Vertex &a);
    void draw(Polygon & p);
    void draw(Vertex &a, Vertex &b);
    
    // Transformations
    void translateXY(Polygon &a, float pos);
    void rotate(Polygon &a, float pos);
    void scale(Polygon &a, float pos);
    
    
    operator float*() { return PixelBuffer; }
    
    // Basic lines
    void horizontalLine(Point &a, Point &b);
    void verticalLine(Point &a, Point &b);
    void diagonalLinePositive(Point &a, Point &b);
    void diagonalLineNegative(Point &a, Point &b);
    
    
    // Bresenham functions
    void bresenham(Point &a, Point &b, int &delta_x, int &delta_y);
    void bresenhamLTPO(Point &a, Point &b, int &delta_x, int &delta_y); // Less than positve one (postive slope)
    void bresenhamGTPO(Point &a, Point &b, int &delta_x, int &delta_y); // Greater than positive one (postive slope)
    void bresenhamLTNO(Point &a, Point &b, int &delta_x, int &delta_y); // Less than negative one (negative slope)
    void bresenhamGTNO(Point &a, Point &b, int &delta_x, int &delta_y); // Greater than negative one (negative slope)

    // Digital Differential Analyzer
    void digitalDifferentialAnalyzer(Point &a, Point &b, int &delta_x, int &delta_y);
    
};
#endif /* Draw_hpp */
