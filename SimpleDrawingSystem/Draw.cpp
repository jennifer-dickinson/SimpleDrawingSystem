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


void Draw::draw() {
    for(int i = 0; i < x * y * 3; i++) PixelBuffer[i] = 0;
    for(Polygon &poly: polygons) draw(poly);
}

void Draw::draw(Polygon & p) {
    /*
     * Draw a polygon by drawing a connecting line between sequential vertices.
     */
    for (Point &point: p.point) {
        point.xd = (point.xr - x_min) / delta;
        point.yd = (point.yr - y_min) / delta;
    }
    for(int i = 0; i < p.point.size() - 1; i++) {
//        std::cout << "    Drawing " << p.point[i] << " to " << p.point[i+1] << std::endl;
        draw(p.point[i], p.point[i+1]);
    }
    
    // This draws a line between the last and first vertices
    if (p.point.front().x != p.point.back().x && p.point.front().y != p.point.back().y) {
//        std::cout << "    Drawing " << p.point.front() << " to " << p.point.back() << std::endl;
        draw(p.point.back(), p.point.front());
    }
}

void Draw::draw(Vertex &a, Vertex &b) {
    float deltax, deltay;
    // Make sure to convert the vertices from device to pixel coordinates.
    xd2xp(a); xd2xp(b);
    
    // Order the vertices from left to right, bottom to top
    Point min, max;
    if(a.xr < b.xr) {
        min = a;
        max = b;
    } else if (b.xr < a.xr) {
        min = b;
        max = a;
    } else if (a.yr < b.yr) {
        min = a;
        max = b;
    } else {
        min = b;
        max = a;
    }
    
    // Find the change in x and y
    deltax = max.xr - min.xr;
    deltay = max.yr - min.yr;
    
//    std::cout << deltax << " " << deltay << std::endl;
    if(deltax == 0) verticalLine(min, max);
    else if (deltay == 0) horizontalLine(min, max);
    else if (deltay == deltax)  diagonalLinePositive(min, max);
    else if (deltay == - deltax)  diagonalLineNegative(min, max);
    else if (bresenhamAlgo)  bresenham(min, max, deltax, deltay);
    else digitalDifferentialAnalyzer(min, max, deltax, deltay);
}

