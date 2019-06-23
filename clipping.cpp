//
//  clipping.cpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/22/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

#include "Draw.hpp"

float intersection (const Point &start, const Point &end, const float &xLine) {
    if (end.xr == start.xr) {
        return start.yr;
    }
    float slope = (end.yr - start.yr) / (end.xr - start.xr);
    float b = end.yr - slope * end.xr;
    return slope * xLine + b;
}

void Draw::CohenSutherland(Polygon &poly) {
    /*
     * Uses the Cohen Sutherland algorithm to determine if a polygon
     * needs to be clipped.
     */

    // First locate each worldPoint.
    char  A = y_max, B = y_min, R = x_max, L = y_min;
    for(Point &p: poly) {
        locate(p);
        A &= p.region;
        B &= p.region;
        R &= p.region;
        L &= p.region;
    }

    if (A || B || R || L) { // All points above, or below, or left, or right.
        poly.resize(0);
        return;
    }

    // Now we do the algoirthm
    for (int j = 0; j < 2; j++)
    for (int i = 0; i < 4; i++) {
        // Check for each boundary x_min, y_max, x_max, y_min
        char OOBR = 1 << i; // OOBR = Out Of Bounds Region
        for (int p = 0; p < poly.size(); p++) {

            if (poly[p].region & OOBR) {
                // Find two replacement points and delete the old point
                ClippingPointAddOn(poly, p, 1, OOBR);
                ClippingPointAddOn(poly, p, -1, OOBR);
                poly.erase(poly.begin()+p);
            }
        }
    }

    assert(poly.size() <= 6);
}

void Draw::ClippingPointAddOn(Polygon &poly, const int &p, const int &pnext, char OOBR) {
    /*
    Logic:
        1. Check if the next reference vertex is also out of
        bounds in the same region. If it is, skip calculating
        for it, and move on to step 4.
        2. Calculate the intersection of the boundry line with
        the line of the current vertex and the reference vertex.
        3. Insert the intersecting point after the current
        vertex.
    */
    Point *newPoint = NULL;
    float intercept = 0;
    if (!(poly[p+pnext].region & OOBR)) {
        if (OOBR & x_min || OOBR & x_max) {
            intercept = intersection(poly[p], poly[p+pnext], ViewBox[OOBR]);
            newPoint =  new Point(ViewBox[OOBR], intercept);
        } else {
            intercept = intersection(poly[p].T(), poly[p+pnext].T(), ViewBox[OOBR]);
            newPoint =  new Point(intercept, ViewBox[OOBR]);
        }
        locate(*newPoint);
        poly.insert(poly.begin()+p+1, *newPoint);
    }
}

void Draw::locate(Point &p) {

    /*
     * Assigns a region to a worldPoint relative to the viewbox.
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
