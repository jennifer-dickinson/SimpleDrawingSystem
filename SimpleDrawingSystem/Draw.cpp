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
    p.xp = p.xd * (min - 1);
    p.yp = p.yd * (min - 1);
    
//    std::cout << "(" << p.xd << "," << p.yd << ")" << " -> (" << p.xp << "," << p.yp << ")" << std::endl;
//    if (p.x != 0 && x != 1) assert(p.xp != p.xd && p.xp != p.x);

}

void Draw::MakePix (const Point &a) {
    MakePix(a.xp, a.yp);
}

void Draw::MakePix (const int &_x, const int &_y) {
//    std::cout << "Pixel: " <<  ( _x + _y * x)  << std::endl;
    PixelBuffer[3 * ( _x + _y * x) ] = 1.0;
    PixelBuffer[3 * ( _x + _y * x) + 1] = 1.0;
    PixelBuffer[3 * ( _x + _y * x) + 2] = 1.0;
}

void Draw::draw (Vertex &a) {
    xd2xp(a);
    MakePix(a.xp, a.yp);
}

void Draw::draw(Polygon & p) {
    for(int i = 0; i < p.point.size() - 1; i++) {
        draw(p.point[i], p.point[i+1]);
    }
    if (&p.point.front() != &p.point.back()) {
        draw(p.point.front(), p.point.back());
    }
}

void Draw::draw(Vertex &a, Vertex &b) {
    
    std::cout << a << " to " << b << ": ";
    xd2xp(a);
    xd2xp(b);
    
    float delta_x = a.x - b.x;
    float delta_y = a.y - b.y;
    
    printf("%.2f %.2f ", delta_x, delta_y);
    
    if(delta_x == 0) {
        // Vertical line
        std::cout << "V line" << std::endl;

        int min = std::min(a.yp, b.yp);
        int max = std::max(a.yp, b.yp);
        
        
        for (int i = min; i < max + 1; i++) MakePix(a.xp, i);
        
    } else if (delta_y == 0) {
        // Horizontal line
        std::cout << "H line" << std::endl;

        
        int min = std::min(a.xp, b.xp);
        int max = std::max(a.xp, b.xp);
        
        for (int i = min; i < max + 1; i++) MakePix(i, a.yp);
        
    } else if (delta_y == delta_x) {
        // Diagonal line
        std::cout << "D line" << std::endl;
        Point min(a), max(b);
        if (a.x > b.x) {
            max = a;
            min = b;
        }
        int diff = max.xp - min.xp;
        
        std::cout << "Min: " << min << " Max: " << max << " Diff: " << diff << std::endl;
        for(int i = 0; i < diff + 1; i++) MakePix(min.xp + i, min.yp + i);


    } else if (delta_y == - delta_x) {
        std::cout << "-D line" << std::endl;
        Point min(a), max(b);
        if (a.x > b.x) {
            max = a;
            min = b;
        }
        int diff = max.xp - min.xp;
        
        std::cout << "Min: " << min << " Max: " << max << " Diff: " << diff << std::endl;
        for(int i = 0; i < diff + 1; i++) MakePix(min.xp + i, min.yp - i);


    } else {
        std::cout << "S line" << std::endl;
    }
    
    
    // Use DDA or other algoirthm to draw a lien from vertex a to vertix b
}

