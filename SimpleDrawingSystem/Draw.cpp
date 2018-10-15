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
    for(int i = 0; i < p.point.size() - 1; i++) draw(p.point[i], p.point[i+1]);
    
    // This connects the last vertex with the first vertex
    if (&p.point.front() != &p.point.back()) draw(p.point.front(), p.point.back());
}

void Draw::draw(Vertex &a, Vertex &b) {
    
    // Make sure to convert the vertices from device to pixel coordinates.
    xd2xp(a); xd2xp(b);
    
    // Find the change in x and y
    int delta_x = b.x - a.x, delta_y = b.y - a.y;
    
    
    if(delta_x == 0) {
        // Vertical line

        int min = std::min(a.yp, b.yp);
        int max = std::max(a.yp, b.yp);
        
        for (int i = min; i < max + 1; i++) MakePix(a.xp, i);
        
    } else if (delta_y == 0) {
        // Horizontal line
        
        int min = std::min(a.xp, b.xp);
        int max = std::max(a.xp, b.xp);
        
        for (int i = min; i < max + 1; i++) MakePix(i, a.yp);
        
    } else if (delta_y == delta_x) {
        // Slope is exactly 1
        Point min = (a.xp > b.xp)? b : a;
        Point max = (a.xp < b.xp)? b : a;
        
        int diff = max.xp - min.xp;
        
        for(int i = 0; i < diff + 1; i++) MakePix(min.xp + i, min.yp + i);


    } else if (delta_y == - delta_x) {
        // Slope is exactly -1
        Point min = (a.xp > b.xp)? b : a;
        Point max = (a.xp < b.xp)? b : a;
        
        int diff = max.xp - min.xp;
        
        for(int i = 0; i < diff; i++) MakePix(min.xp + i, min.yp - i);


    } else if ( (delta_y / (float)delta_x) < 1 && (delta_y / (float) delta_x) > 0){
        // Bresenham when m < 1 and m > 0
        std::cout << "Bresenham - slope: " <<  (delta_y / (float) delta_x) << std::endl;
    
        
        int y_ = a.yp, y_prev = a.yp;
        int p = 2 * delta_y - delta_x, p_;
        if (!(p < 0)) {
            y_prev = a.yp;
            y_++;
            std::cout << "Increment" << std::endl;
        }
        MakePix(a);
        MakePix(a.xp + 1, y_);
        
        for(int i = a.xp + 2; i < b.xp; i++) {
            MakePix(i, y_);
            
            p_ = p;
            p = p_ + 2 * delta_y - 2 * delta_x * (y_ - y_prev);

            printf("%d + 2 * %d - 2 * %2d (%d - %d) = %d\n", p_, delta_y, delta_x, y_, y_prev, p);
            y_prev = y_;
            if (!(p < 0)) {
                y_++;
            }
        }
    } else {
        
        // Bresenham when
        std::cout << "-S line" << std::endl;
    }
    
    // Use DDA or other algoirthm to draw a lien from vertex a to vertix b
}

