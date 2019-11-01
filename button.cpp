#include "button.hpp"

void Button::click(){
    action();
}

float Button::r(){
    return this->active? this->active_color[0]: this->inactive_color[0];
}

float Button::g(){
    return this->active? this->active_color[1]: this->inactive_color[1];
}

float Button::b(){
    return this->active? this->active_color[2]: this->inactive_color[2];
}

Button::Button(int x, int y, int w, int h,std::string text, void(*action)(void)):
 x(x), y(y), w(w), h(h),
 text(text), action(action)
{
    this->active_color = {.5,.5,.5};
    this->inactive_color = {.2,.2,.2};
    this->active = false;

    poly.addVertex(Point(x,y));
    poly.addVertex(Point(x+w, y));
    poly.addVertex(Point(x+w,y+h));
    poly.addVertex(Point(x,y+h));

}

bool Button::bounded(const int &x_ref, const int &y_ref) {
    bool bounded = true;
    if (x_ref < this->x && x_ref > this->x + this->w) bounded = false;
    if (y_ref < this->y && y_ref > this->y + this->h) bounded = false;
    return bounded;
}