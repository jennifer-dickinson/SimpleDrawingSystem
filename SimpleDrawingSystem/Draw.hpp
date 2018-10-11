//
//  Draw.hpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/9/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

#ifndef Draw_hpp
#define Draw_hpp

#include <stdio.h>
#include "polygon.h"


class Draw {
    unsigned int x;
    unsigned int y;
    float *PixelBuffer;
    int min;
    
    enum sett {};
public:
    Draw() : x(640), y(480) {
        PixelBuffer = new float[(x+1) * (y+1) * 3];
        min = std::min(x,y);
    }
    Draw(unsigned int _x, unsigned int _y) : x(_x), y(_y) {
        PixelBuffer = new float[(x+1) * (y+1) * 3];
        min = std::min(x,y);
    }
    
    void digitalDifferenceAnalyzer(Point p1, Point p2);
    
    void MakePix (const Point &a);
    
    void MakePix (const int &x, const int &y);
    
    void draw(Vertex &a);
    
    void xd2xp(Point &p);
    
    void draw(Polygon & p);
    
    void draw(Vertex &a, Vertex &b);
    
    operator float*() { return PixelBuffer; }
    
};
#endif /* Draw_hpp */
