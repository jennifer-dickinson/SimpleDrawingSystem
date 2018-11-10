//
//  Bresenham.cpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/14/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

#include "Draw.hpp"

void Draw::bresenham(Point &min, Point &max, float delta_x, float delta_y) {
    delta_x = (max.xp - min.xp);
    delta_y = (max.yp - min.yp);
    float slope = (delta_y / (float) delta_x);
    if (slope > 1) bresenhamGTPO(min, max, delta_x, delta_y);
    else if (slope < -1) bresenhamLTNO(min, max, delta_x, delta_y);
    else if (slope > 0) bresenhamLTPO(min, max, delta_x, delta_y);
    else bresenhamGTNO(min, max, delta_x, delta_y);
}
void Draw::bresenhamLTPO(Point &min, Point &max, float &delta_x, float &delta_y) {
    // Bresenham when slope < 1 && slope > 0

    int y_ = min.yp, y_prev = min.yp, p = 2 * delta_y - delta_x;

    for(int i = min.xp + 2; i <= max.xp; i++) {
        MakePix(i, y_);
        p = p + 2 * delta_y - 2 * delta_x * (y_ - y_prev);
        y_prev = y_;
        if (!(p < 0)) y_++;
    }
} // Less than positve one (postive slope)

void Draw::bresenhamGTPO(Point &min, Point &max, float &delta_x, float &delta_y) {
    // Bresenham slope > 1

    int x_ = min.xp, x_prev = min.xp, p = 2 * delta_x - delta_y;

    for(int i = min.yp + 1; i <= max.yp; i++) {
        MakePix(x_, i);
        p = p + 2 * delta_x - 2 * delta_y * (x_ - x_prev);
        x_prev = x_;
        if (!(p < 0)) x_++;
    }
} // Greater than positive one (postive slope)

void Draw::bresenhamLTNO(Point &min, Point &max, float &delta_x, float &delta_y) {
    // Bresenham when slope < -1

    int x_ = min.xp, x_prev = min.xp, p = 2 * delta_x - delta_y;

    for(int i = min.yp - 1; i >= max.yp && !(x_ < 0); i--) {
        MakePix(x_, i);
        p = p + 2 * delta_x + 2 * delta_y * (x_ - x_prev);
        x_prev = x_;
        if (!(p < 0)) x_++;
    }
} // Less than negative one (negative slope)

void Draw::bresenhamGTNO(Point &min, Point &max, float &delta_x, float &delta_y) {
    // Bresenham when slope > -1 && slope < 0
    int y_ = min.yp, y_prev = min.yp, p = 2 * delta_y - delta_x;

    for(int i = min.xp + 1; i <= max.xp && !(y_ < 0); i++) {
        MakePix(i, y_);
        p = p + 2 * delta_y - 2 * delta_x * (y_ - y_prev);
        y_prev = y_;
        if (p < 0) y_--;
    }

} // Greater than negative one (negative slope)
