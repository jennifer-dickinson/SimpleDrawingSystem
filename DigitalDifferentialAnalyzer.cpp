//
//  digitalDifferenceAnalyzer.cpp
//  DrawingSYstem
//
//  Created by Jennifer Salas on 10/21/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

#include <stdio.h>
#include "Draw.hpp"


void Draw::digitalDifferentialAnalyzer(const Point &start, const Point &end, float &delta_x, float &delta_y) {
    float slope = delta_y /  delta_x;
    if (abs(slope) > 1) DDAGreater(start, end, slope);
    else DDARegular(start, end, slope);
}

void Draw::DDAGreater(const Point &start, const Point &end, const float &slope) {
    Point start2 = start, end2 = end;
    if (end.yp < start.yp) std::swap(start2, end2);

    int start_y = start2.yp, end_y = end2.yp;
    int start_x = start2.xp;
    int b = start_y - slope * start_x;

//    assert(start_y <= end_y);

    for(int i = start_y; i <= end_y - 1; i++) {
        int x_value = std::round((i - b) / slope);
//        assert(x_value >= 0);
        MakePix(x_value, i);
    }

    MakePix(end2);

}

void Draw::DDARegular(const Point &start, const Point &end, const float &slope) {
    Point start2 = start, end2 = end;

    if (end2.xp < start2.xp) {
        std::swap(start2, end2);
    }

    int start_x = start2.xp, end_x = end2.xp;
    int start_y = start2.yp;
    int b = start_y - slope * start_x;

//    assert(start_x <= end_x);


    for(int i = start_x; i <= end_x - 1; i++) {
        int y_value = std::round(b + slope * i);
        MakePix(i, y_value);
    }
    MakePix(end2);
}
