//
//  Draw.cpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/9/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

#include "Draw.hpp"
#include <limits>
#include <cassert>


void Draw::xd2xp(Point &p) {
    /*
     * Convert device coordinates to pixel coordinates
     */

    if (view == XZ) {
        p.xd = (p.xr - ViewBox[x_min]) / delta;
        p.yd = (p.yr - ViewBox[z_min]) / delta;
    } else if(view == YZ) {
        p.xd = (p.xr - ViewBox[y_min]) / delta;
        p.yd = (p.yr - ViewBox[z_min]) / delta;
    } else {
        p.xd = (p.xr - ViewBox[x_min]) / delta;
        p.yd = (p.yr - ViewBox[y_min]) / delta;
    }

    p.xp = (int) ((p.xd * .8 + .1) * (min - 1));
    p.yp = (int) ((p.yd * .8 + .1) * (min - 1));
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
    if (ThreeDimensional) for(Polyhedron &poly: polyhedrons) draw(poly);
    else for(Polygon &poly: polygons) draw(poly);
}

void Draw::draw(Polygon & p) {
    /*
     * Draw a polygon by drawing a connecting line between sequential vertices.
     */

    // Clip the polygon here
    Polygon c = p; // c means clipped

    CohenSutherland(c);

    if (raster && c.size() > 2) rasterize(c);
//
    // Draw the points of the polygon.
    for(int i = 0; i < c.size() ; i++) draw(c[i], c[i+1]);
}


int intersection(float delta_x, float delta_y, int y_line, Point &point) {
    float slope = delta_y / delta_x;
    float b = point.yp - slope * point.xp;
    return (y_line - b) / slope;
}

template<typename t>
bool sameSign(t num1, t num2) {
    return !((num2 >= 0 && num1 <= 0) || (num2 <= 0 && num1 >= 0));
}

void Draw::rasterize(Polygon &c) {
    // This must receive clipped polygon
    std::cout << "Rasterizing clipped polygon" << std::endl;
    int x_min = std::numeric_limits<int>::max(),
        x_max = std::numeric_limits<int>::min(),
        y_min = std::numeric_limits<int>::max(),
        y_max = std::numeric_limits<int>::min();

    // calculate the pixel extrema in x and y
    for (int i = 0; i < c.size() + 1; i++) {
        xd2xp(c[i]);
        if (c[i].xp > x_max) x_max = c[i].xp;
        if (c[i].xp < x_min) x_min = c[i].xp;
        if (c[i].yp > y_max) y_max = c[i].yp;
        if (c[i].yp < y_min) y_min = c[i].yp;
    }

    // x_max++;
    // x_min--;
    // y_max++;
    // y_min--;

    // now fill int the pixels row by row;
    int prevstat = -1;

    for(int y_ = y_min; y_ < y_max; y_++) {
        std::vector<int> xs;

        // calculate the intersections of every line in the polygon
        for(int i = 0 ; i < c.size(); i++) {

            if(std::max(c[i].yp, c[i+1].yp) >= y_ && std::min(c[i].yp, c[i+1].yp) <= y_) {
                float delta_x = c[i].xp - c[i+1].xp, delta_y = c[i].yp - c[i+1].yp;
                int x_point;
                if (delta_x == 0) {
                    x_point = c[i].xp ;
                }
                else if (delta_y != 0){
                    x_point = intersection(delta_x, delta_y, y_, c[i]);
                    if (abs(c[i].xp -  x_point) < 0.001 && !(sameSign(c[i - 1].yp - c[i].yp, c[i + 1].yp - c[i].yp))) continue;  //TODO: Fix line not showing on vertical slope
                    // check for x extrema
                }
                xs.push_back(x_point);
            }
        }

        // sort all the points
        std::sort(xs.begin(), xs.end());
        auto current = xs.begin();
        bool on = false;

        for(int x_ = x_min; x_ <= x_max && current != xs.end() ; x_++) {
            if(x_ == *current) {
                on = !on;
                current++;
                x_--;
            }
            else if (on) MakePix(x_,y_);
        }


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

//    std::cout << a<< b << deltax << " " << deltay << std::endl;
    if(deltax == 0) verticalLine(min, max);
    else if (deltay == 0) horizontalLine(min, max);
    else if (deltay == deltax)  diagonalLinePositive(min, max);
    else if (deltay == - deltax)  diagonalLineNegative(min, max);
    else if (bresenhamAlgo)  bresenham(min, max, deltax, deltay);
    else digitalDifferentialAnalyzer(min, max, deltax, deltay);
}

void Draw::draw(Polyhedron::Point3D &a_, Polyhedron::Point3D &b_) {
    // Handles cavalier and cabinet views
    Point a, b;
    if (view == XY) {
        a = a_.xy();
        b = b_.xy();
    } else if (view == XZ) {
        a = a_.xz();
        b = b_.xz();
    } else {
        a = a_.yz();
        b = b_.yz();
    }

    draw(a,b);

}
