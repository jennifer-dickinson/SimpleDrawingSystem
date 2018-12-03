#ifndef HELPERS
#define HELPERS


#include <fstream>
#include <iostream>
#include <utility>
#include <cmath>

enum View {XY, XZ, YZ, CAVALIER, CABINET};

float niceround(float num);

void getinput(std::ifstream &input, std::string &line);

#endif //#HELPERS
