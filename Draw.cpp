//
//  Draw.cpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/9/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

#include "Draw.hpp"
#include <limits>


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

    p.xp = (int) std::round((p.xd * .8 + .1) * (min - 1) * 100) / 100;
    p.yp = (int) std::round((p.yd * .8 + .1) * (min - 1) * 100) / 100;
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

    rasterize(c);

    // Draw the points of the polygon.
    for(int i = 0; i < c.size() ; i++) draw(c[i], c[i+1]);
}


int intersection(Point &a, Point &b, int y_line) {
    float delta_x = b.x - a.x;
    if (delta_x == 0) return b.xp;
    float slope = (b.y - a.y) / delta_x;
    int y_inter = static_cast<int>(b.yp - slope * b.xp);
    return static_cast<int>(std::round(((y_line - y_inter) / slope) * 100) / 100);
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

    // now fill int the pixels row by row;

    for(int y_ = y_min; y_ < y_max; y_++) {
        std::vector<int> xs;

        // calculate the intersections of every line in the polygon
        for(int i = 0 ; i < c.size(); i++) {
            int x_point = intersection(c[i], c[i+1], y_);

            if (x_point <= std::max(c[i].xp, c[i+1].xp) && x_point >= std::min(c[i].xp, c[i+1].xp))
                xs.push_back(x_point);
        }

        if (xs.size() > 1) {
            // sort all the points
            std::sort(xs.begin(), xs.end());

            for(auto inter = xs.begin(); inter < xs.end(); inter += 2) {
                for(int x_ = *inter; x_ < *(inter + 1); x_++) {
                    MakePix(x_, y_);
                }
            }
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
