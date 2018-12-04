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

    // std::cout << "Translating point from real to device to pixel" << std::endl;
    // std::cout << "  (" << p.xr << "," << p.yr << ") : (" << p.xd << "," << p.yd << ") : (" << p.xp << "," << p.yp << ")" << std::endl;
}

void Draw::MakePix (const Point &a) {
    /*
     * Alternate method of drawing a pixel on a screen
     */
    MakePix(a.xp, a.yp);
}

void Draw::MakePix (const int & _x, const int &_y) {
    float r =1, g = 1, b = 1;
    MakePix(_x, _y, 1, 1, 1);
}

void Draw::MakePix (const int &_x, const int &_y, float r, float g, float b) {
    /*
     * Basic method of drawing a pixel on the screen. 3 sequential floats represent RGB values.
     */
    PixelBuffer[3 * ( _x + _y * x) ] = r;
    PixelBuffer[3 * ( _x + _y * x) + 1] = g;
    PixelBuffer[3 * ( _x + _y * x) + 2] = b;
}

void Draw::draw (Vertex &a) {
    /*
     * Draw a single vertex by converting its device coordinates to pixel coordinates.
     */
    xd2xp(a);
    MakePix(a.xp, a.yp, a.r, a.g, a.b);
}


void Draw::draw() {
    for(int i = 0; i < x * y * 3; i++) PixelBuffer[i] = 0;
    if (Shader) {
        sortShadedPolyhedrons();
        for(ShadedPolyhedron &poly: sPolyhedrons) draw(poly);
    }
    else if (ThreeDimensional) for(Polyhedron &poly: polyhedrons) draw(poly);
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
    if (!raster) for(int i = 0; i < c.size() ; i++) draw(c[i], c[i+1]);
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
    // std::cout << "Rasterizing clipped polygon" << std::endl;

    // std::cout << "Polygon colors are:: " << std::endl;
    // for (auto &p: c) {
    //     std::cout << p.r  << " " << p.g << " " << p.b << std::endl;
    // }
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


    for(int y_ = y_min; y_ < y_max; y_++) {
        std::vector<std::pair<int, std::vector<float> > > xs;

        // calculate the intersections of every line in the polygon
        for(int i = 0 ; i < c.size(); i++) {

            if(std::max(c[i].yp, c[i+1].yp)  >= y_  && std::min(c[i].yp, c[i+1].yp) <= y_) {
                float delta_x = c[i].xp - c[i+1].xp, delta_y = c[i].yp - c[i+1].yp;
                int x_point;
                if (delta_x == 0) {
                    x_point = c[i].xp ;
                }
                else {
                    x_point = intersection(delta_x, delta_y, y_, c[i]);
                }
                std::vector<float> rgb(3);

                // Calculate RGB values at this point.
                // std::cout << x_point << " " << c[i].xp << " " << y_ << " " << c[i].yp << std::endl;
                float d1 = sqrtf(powf(x_point - c[i].xp, 2) + powf(y_ - c[i].yp, 2));
                float d2 = sqrtf(powf(x_point - c[i+1].xp, 2) + powf(y_ - c[i+1].yp, 2));
                // std::cout << d1 << " " << d2 << std::endl;
                float tot = d1 + d2;
                d1 /= tot;
                d2 /= tot;
                // assert(d1 != 0);
                // assert(d2 != 0);
                float delta;
                // std::cout << "RGB: (" << c[i].r << " " << c[i].g << " " << c[i].b << ") (" << c[i+1].r << " " << c[i+1].g << " " << c[i+1].b << ")" << std::endl;;
                if (d1 == 0) {
                    rgb[0] = c[i].r;
                    rgb[1] = c[i].g;
                    rgb[2] = c[i].b;
                } else if (d2 == 0) {
                    rgb[0] = c[i + 1].r;
                    rgb[1] = c[i + 1].g;
                    rgb[2] = c[i + 1].b;
                } else {
                    if(c[i].r > c[i+1].r) {
                        delta =  c[i].r - c[i+1].r;
                        rgb[0] = delta * d2 + c[i+1].r;
                    } else {
                        delta =  c[i].r - c[i+1].r;
                        rgb[0] = delta * d1 + c[i+1].r;
                    }
                    if(c[i].g > c[i+1].g) {
                        delta =  c[i].g - c[i+1].g;
                        rgb[1] = delta * d2 + c[i+1].g;
                    } else {
                        delta =  c[i].g - c[i+1].g;
                        rgb[1] = delta * d1 + c[i+1].g;
                    }
                    if(c[i].b > c[i+1].b) {
                        delta =  c[i].b - c[i+1].b;
                        rgb[2] = delta * d2 + c[i+1].b;
                    } else {
                        delta =  c[i].b - c[i+1].b;
                        rgb[2] = delta * d1 + c[i+1].b;
                    }

                }
                std::pair<int, std::vector<float> > an_x(x_point, rgb);
                xs.push_back(an_x);
            }
        }

        // sort all the points
        std::sort(xs.begin(), xs.end());

        auto current = xs.begin();
        bool on = false;
        float old_r = 0, old_g = 0, old_b = 0;
        float man_r = 0, man_g = 0, man_b = 0;
        float cur_r = current->second[0], cur_g = current->second[1], cur_b = current->second[2];
        float dr, dg, db;

        for(int x_ = x_min; x_ <= x_max && current != xs.end() ; x_++) {
            if(x_ == current->first) {
                on = !on;
                current++;
                x_--;
                if(current != xs.end() && next(current) != xs.end()) {

                    old_r = man_r = cur_r;
                    old_g = man_g = cur_g;
                    old_b = man_b = cur_b;
                    cur_r = next(current)->second[0];
                    cur_g = next(current)->second[1];
                    cur_b = next(current)->second[2];

                    // if(current->first != std::next(current)->first) {
                        dr = (cur_r - old_r) / (next(current)->first - current->first);
                        dg = (cur_g - old_g) / (next(current)->first - current->first);
                        db = (cur_b - old_b) / (next(current)->first - current->first);
                        std::cout << "Delta values for rgb: " << dr << " " << dg << " " << db << std::endl;
                    // } else  dr = dg = db = 0;

                }
                // printf("Old color: %.2f %.2f %.2f; New color: %.2f %.2f %.2f. \n", old_r, old_g, old_b, cur_r, cur_g, cur_b);

            }
            else if (on) {
                // calculate the color
                old_r += dr;
                old_b += db;
                old_g += dg;

                MakePix(x_,y_, old_r, old_g, old_b);
            }
        }
        // std::cout << std::endl;
        // std::cout << "Did y " << y_ << std::endl;

    }

    // for(int x_ = x_min; x_ < x_max; x_++) {
    //     std::vector<std::pair<int, std::vector<float> > > ys;
    //
    //     // calculate the intersections of every line in the polygon
    //     for(int i = 0 ; i < c.size(); i++) {
    //
    //         if(std::max(c[i].xp, c[i+1].xp) + 1  >= x_  && std::min(c[i].xp, c[i+1].xp) - 1 <= x_) {
    //             float delta_y = c[i].yp - c[i+1].yp, delta_x = c[i].xp - c[i+1].xp;
    //             int y_point;
    //             if (delta_y == 0) {
    //                 y_point = c[i].yp ;
    //             }
    //             else {
    //                 auto inversePoint = c[i];
    //                 std::swap(inversePoint.xp, inversePoint.yp);
    //                 y_point = intersection(delta_y, delta_x, x_, inversePoint);
    //             }
    //             std::vector<float> rgb(3);
    //
    //             // Calculate RGB values at this point.
    //             std::cout << y_point << " " << c[i].xp << " " << x_ << " " << c[i].yp << std::endl;
    //             float d1 = sqrtf(powf(x_ - c[i].xp, 2) + powf(y_point - c[i].yp, 2));
    //             float d2 = sqrtf(powf(x_ - c[i+1].xp, 2) + powf(y_point - c[i+1].yp, 2));
    //             std::cout << d1 << " " << d2 << std::endl;
    //             float tot = d1 + d2;
    //             d1 /= tot;
    //             d2 /= tot;
    //             // assert(d1 != 0);
    //             // assert(d2 != 0);
    //             float delta;
    //             std::cout << "RGB: (" << c[i].r << " " << c[i].g << " " << c[i].b << ") (" << c[i+1].r << " " << c[i+1].g << " " << c[i+1].b << ")" << std::endl;;
    //             if(c[i].r > c[i+1].r) {
    //                 delta =  c[i].r - c[i+1].r;
    //                 rgb[0] = delta * d2 + c[i+1].r;
    //             } else {
    //                 delta =  c[i].r - c[i+1].r;
    //                 rgb[0] = delta * d1 + c[i+1].r;
    //             }
    //             if(c[i].g > c[i+1].g) {
    //                 delta =  c[i].g - c[i+1].g;
    //                 rgb[1] = delta * d2 + c[i+1].g;
    //             } else {
    //                 delta =  c[i].g - c[i+1].g;
    //                 rgb[1] = delta * d1 + c[i+1].g;
    //             }
    //             if(c[i].b > c[i+1].b) {
    //                 delta =  c[i].b - c[i+1].b;
    //                 rgb[2] = delta * d2 + c[i+1].b;
    //             } else {
    //                 delta =  c[i].b - c[i+1].b;
    //                 rgb[2] = delta * d1 + c[i+1].b;
    //             }
    //
    //             std::pair<int, std::vector<float> > an_y(y_point, rgb);
    //             ys.push_back(an_y);
    //         }
    //     }
    //
    //     // sort all the points
    //     std::sort(ys.begin(), ys.end());
    //
    //     auto current = ys.begin();
    //     bool on = false;
    //     float old_r = 0, old_g = 0, old_b = 0;
    //     float man_r = 0, man_g = 0, man_b = 0;
    //     float cur_r = current->second[0], cur_g = current->second[1], cur_b = current->second[2];
    //     float dr, dg, db;
    //
    //     for(int y_ = y_min; y_ <= y_max && current != ys.end() ; y_++) {
    //         if(y_ == current->first) {
    //             on = !on;
    //             current++;
    //             y_--;
    //             if(current != ys.end() && next(current) != ys.end()) {
    //
    //                 old_r = man_r = cur_r;
    //                 old_g = man_g = cur_g;
    //                 old_b = man_b = cur_b;
    //                 cur_r = next(current)->second[0];
    //                 cur_g = next(current)->second[1];
    //                 cur_b = next(current)->second[2];
    //
    //                 if(current->first != std::next(current)->first) {
    //                     dr = (cur_r - old_r) / (next(current)->first - current->first);
    //                     dg = (cur_g - old_g) / (next(current)->first - current->first);
    //                     db = (cur_b - old_b) / (next(current)->first - current->first);
    //                 } else  dr = dg = db = 0;
    //
    //             }
    //             printf("Old color: %.2f %.2f %.2f; New color: %.2f %.2f %.2f. \n", old_r, old_g, old_b, cur_r, cur_g, cur_b);
    //
    //         }
    //         else if (on) {
    //             // calculate the color
    //             old_r += dr;
    //             old_b += db;
    //             old_g += dg;
    //
    //             MakePix(x_,y_, old_r, old_g, old_b);
    //         }
    //     }
    // }
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
        max = b; // hello theere
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
    // std::cout << "Drew from " << a.xp << "," << a.yp << " to " << b.xp << "," << b.yp <<std::endl;
}

void Draw::draw(Point3D &a_, Point3D &b_) {
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
