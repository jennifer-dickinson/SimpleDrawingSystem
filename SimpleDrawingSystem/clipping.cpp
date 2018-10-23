//
//  clipping.cpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/22/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

#include "Draw.hpp"

float Draw::Xintersection (const Point &start, const Point &end, const float &yLine) {
    float slope = (end.yr - start.yr) / (end.xr - start.xr);
    if (end.xr - start.xr == 0 ) {
        return end.xr;
    }
    float b = end.yr - slope * end.xr;
    return (yLine - b) / slope;
}
float Draw::Yintersection (const Point &start, const Point &end, const float &xLine) {
    float slope = (end.yr - start.yr) / (end.xr - start.xr);
    if (end.xr - start.xr == 0) {
        return start.yr;
    }
    float b = end.yr - slope * end.xr;
    return slope * xLine + b;
}

void Draw::CohenSutherland(Polygon &poly) {
    /*
     * Uses the CohenSutherland algorithm to determine if a polygon
     * needs to be clipped.
     */
    
    // First locate each point.
    bool bounded = false;
    for(auto &p: poly) {
        locate(p);
        if (p.region == 0) bounded = true;
    }
    
    if (!bounded) poly.resize(0);
    // Now we do the algoirthm
    for (int j = 0; j < 2; j++)
    for (int i = 0; i < 4; i++) {
        char OOBR = 1 << i; // OOBR = Out Of Bounds Region
        for(int p = 0; p < poly.size(); p++) {
            if (poly[p].region & OOBR) {
                Point *p1 = NULL, *p2 = NULL;

                // Point is out of bounds
                if (OOBR & x_min) {
                    if ((poly[p+1].region & x_min) != x_min) {
                        float yInter1 = Yintersection(poly[p], poly[p+1], ViewBox[x_min]);
                        p1 =  new Point(ViewBox[x_min], yInter1);
                        locate(*p1);
                    }
                    if ((poly[p-1].region & x_min) != x_min) {
                        float yInter2 = Yintersection(poly[p], poly[p-1], ViewBox[x_min]);
                        p2 =  new Point(ViewBox[x_min], yInter2);
                        locate(*p2);
                    }
                    
                    if(p1) poly.insert(poly.begin()+p+1, *p1);
                    if(p2) poly.insert(poly.begin()+p+1, *p2);

                    poly.erase(poly.begin()+p);

                } else if (OOBR & x_max) {
                    
                    if ((poly[p+1].region & x_max) != x_max) {
                        float yInter1 = Yintersection(poly[p], poly[p+1], ViewBox[x_max]);
                        p1 = new Point(ViewBox[x_max], yInter1);
                        locate(*p1);
                        
                    }
                    if ((poly[p-1].region & x_max) != x_max) {
                        float yInter2 = Yintersection(poly[p], poly[p-1], ViewBox[x_max]);
                        p2 =  new Point(ViewBox[x_max], yInter2);
                        locate(*p2);
                    }
                    
                    if(p1) poly.insert(poly.begin()+p+1, *p1);
                    if(p2) poly.insert(poly.begin()+p+1, *p2);
                    
                    poly.erase(poly.begin()+p);

                } else if (OOBR & y_min) {
                    if ((poly[p+1].region & y_min) != y_min) {
                        float yInter1 = Xintersection(poly[p], poly[p+1], ViewBox[y_min]);
                        p1 =  new Point(yInter1, ViewBox[x_min]);
                        locate(*p1);
                    }
                    if ((poly[p-1].region & y_min) != y_min) {
                        float yInter2 = Xintersection(poly[p], poly[p-1], ViewBox[y_min]);
                        p2 =  new Point(yInter2, ViewBox[x_min]);
                        locate(*p2);

                    }
                    
                    if(p1) poly.insert(poly.begin()+p+1, *p1);
                    if(p2) poly.insert(poly.begin()+p+1, *p2);
                    
                    poly.erase(poly.begin()+p);
                } else if (OOBR & y_max) {
                    if ((poly[p+1].region & y_max) != y_max) {
                        float yInter1 = Xintersection(poly[p], poly[p+1], ViewBox[y_max]);
                        p1 =  new Point(yInter1, ViewBox[y_max]);
                        locate(*p1);
                        
                    }
                    if ((poly[p-1].region & y_max) != y_max) {
                        float yInter2 = Xintersection(poly[p], poly[p-1], ViewBox[y_max]);
                        p2 =  new Point(yInter2, ViewBox[y_max]);
                        locate(*p2);

                    }
                    if(p1) poly.insert(poly.begin()+p+1, *p1);
                    if(p2) poly.insert(poly.begin()+p+1, *p2);
                    poly.erase(poly.begin()+p);
                }
            }
        }
//        for(auto &p: poly) std::cout << "(" << p.xr << "," << p.yr << ")" << std::endl;
    }
    
//    std::cout << "Clipped polygon" << std::endl;
//    for(auto &p: poly) std::cout << "(" << p.xr << "," << p.yr << ")" << std::endl;
}

void Draw::locate(Point &p) {

    /*
     * Assigns a region to a point relative to the viewbox.
     *
     * The region binary value is of the format 0bABRL, where A is above,
     * B is below, R is right, and L is left of the viewbox.
     */
    
    p.region = 0; // Reset the region
    if (p.xr < ViewBox[x_min]) {
        p.region ^= x_min;
    }

    if (p.xr > ViewBox[x_max] ) {
        p.region ^= x_max;
    }

    if (p.yr < ViewBox[y_min]) {
        p.region ^= y_min;
    }

    if (p.yr > ViewBox[y_max]) {
        p.region ^= y_max;
    }
}
