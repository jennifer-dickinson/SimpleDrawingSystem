
#include "helpers.hpp"

float niceround(float num) {
    return roundf(num * 1000) / 1000;
}

void getinput(std::ifstream &input, std::string &line) {
    do {
        std::getline(input, line);
    } while (line.empty());
}
