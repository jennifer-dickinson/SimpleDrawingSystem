#ifndef BUTTON
#define BUTTON
#include <vector>
#include <string>
#include "polygon.hpp"
#include "Point.hpp"

class Button {
    int x, y, w, h;
    void (*action)(void);
    bool active;
    std::string text;
    std::vector<float> active_color;
    std::vector<float> inactive_color;

public:
    Button(int,int,int,int,std::string, void(*action)(void));
    Polygon poly;

    void click();

    bool bounded(const int &x_ref, const int &y_ref);

    float r();
    float g();
    float b();

};

#endif