#ifndef BUTTON
#define BUTTON
#include <vector>
#include <string>

class Button {
    int x, y, w, h;
    void (*action)(void);
    bool active;
    std::string text;
    std::vector<float> active_color;
    std::vector<float> inactive_color;

    Button(int,int,int,int,std::string, void(*action)(void));

    void click();

    bool bounded(const int &x_ref, const int &y_ref);

    float r();
    float g();
    float b();

};

#endif