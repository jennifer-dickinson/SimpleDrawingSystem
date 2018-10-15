//
//  Draw.cpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/9/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

#include "Draw.hpp"
#include <stdio.h>

void Draw::xd2xp(Point &p) {
    /*
     * Convert device coordinates to pixel coordinates
     */
    p.xp = p.xd * (min - 1);
    p.yp = p.yd * (min - 1);
}

void Draw::MakePix (const Point &a) {
    /*
     * Alternate method of drawing a pixel on a screen
     */
    MakePix(a.xp, a.yp);
}

void Draw::MakePix (const int &_x, const int &_y) {
    /*
     * Basic method of drawing a pixel on the screen. 3 sequential floats represent RGB values.
     */
    PixelBuffer[3 * ( _x + _y * x) ] = 1.0;
    PixelBuffer[3 * ( _x + _y * x) + 1] = 1.0;
    PixelBuffer[3 * ( _x + _y * x) + 2] = 1.0;
}

void Draw::draw (Vertex &a) {
    /*
     * Draw a single vertex by converting its device coordinates to pixel coordinates.
     */
    xd2xp(a);
    MakePix(a.xp, a.yp);
}

void Draw::draw(Polygon & p) {
    /*
     * Draw a polygon by drawing a connecting line between sequential vertices.
     */
    for(auto point: p.point) std::cout << point << std::endl;
    for(int i = 0; i < p.point.size() - 1; i++) draw(p.point[i], p.point[i+1]);
    
    // This draws a line between the last and first vertices
    if (&p.point.front() != &p.point.back()) draw(p.point.back(), p.point.front());
}

void Draw::draw(Vertex &a, Vertex &b) {
    int delta_x, delta_y;
    // Make sure to convert the vertices from device to pixel coordinates.
    xd2xp(a); xd2xp(b);
    
    // Order the vertices from left to right, bottom to top
    Point min, max;
    if(a.x < b.x) {
        min = a;
        max = b;
    } else if (b.x < a.x) {
        min = b;
        max = a;
    } else if (a.y < b.y) {
        min = a;
        max = b;
    } else {
        min = b;
        max = a;
    }
    
    // Find the change in x and y
    delta_x = max.x - min.x;
    delta_y = max.y - min.y;
    
    if(delta_x == 0) verticalLine(min, max);
    else if (delta_y == 0) horizontalLine(min, max);
    else if (delta_y == delta_x)  diagonalLinePositive(min, max);
    else if (delta_y == - delta_x)  diagonalLineNegative(min, max);
    else if (bresenhamAlgo)  bresenham(min, max, delta_x, delta_y);
    else digitalDifferentialAnalyzer(min, max, delta_x, delta_y);
}

