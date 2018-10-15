//
//  Lines.cpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/14/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

#include "Draw.hpp"

void Draw::horizontalLine(Point &min, Point &max) {
    // Horizontal line
    for (int i = min.xp; i < max.xp + 1; i++) MakePix(i, min.yp);
}
void Draw::verticalLine(Point &min, Point &max)  {
    // Vertical line
    for (int i = min.yp; i < max.yp + 1; i++) MakePix(min.xp, i);
}
void Draw::diagonalLinePositive(Point &min, Point &max) {
    // Slope is exactly 1
    int diff = max.xp - min.xp + 1;
    for(int i = 0; i < diff; i++) MakePix(min.xp + i, min.yp + i);
}
void Draw::diagonalLineNegative(Point &min, Point &max) {
    // Slope is exactly -1
    int diff = max.xp - min.xp;
    for(int i = 0; i < diff; i++) MakePix(min.xp + i, min.yp - i);
}
