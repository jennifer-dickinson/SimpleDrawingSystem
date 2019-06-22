//
// Created by Jennifer Salas on 10/2/18.
//

#include "Draw.hpp"
#include "helpers.hpp"


void Draw::initializePolygons(std::string file) {
    std::ifstream input(file);
    std::string line;

    if (!input) {
        std::cout << "Could not find file: " << file << std::endl;
        std::exit(0);
    }

    int numPolygons = 0, vertices = 0;
    float x = 0.0, y = 0.0;

    std::getline(input, line);
    std::istringstream ss(line);

    ss >> numPolygons;

    std::cout << "Total polygons: " << numPolygons << std::endl;

//    float x_min = std::numeric_limits<float>::max(), y_min = std::numeric_limits<float>::max();
//    float x_max = std::numeric_limits<float>::min(), y_max = std::numeric_limits<float>::min();

    ViewBox[x_min] = std::numeric_limits<float>::max();
    ViewBox[y_min] = std::numeric_limits<float>::max();
    ViewBox[x_max] = std::numeric_limits<float>::min();
    ViewBox[y_max] = std::numeric_limits<float>::min();


    for (int i = 0; i < numPolygons; i++) {

        std::cout << "Generating polygon #" << i << std::endl;

        getline(input, line);
        getline(input, line);

        std::istringstream vertexCount(line);
        vertexCount >> vertices;

        Polygon temp;

        for (int j = 0; j < vertices; j++) {
            getline(input, line);
            std::istringstream point(line);
            point >> x >> y;

            Point tempPoint (x,y);
//            if (temp.worldPoint.size()) assert(&tempPoint.x != &temp.worldPoint.back().x);
            temp.addVertex(tempPoint);

            if (x > ViewBox[x_max]) ViewBox[x_max] = x;
            if (x < ViewBox[x_min]) ViewBox[x_min] = x;
            if (y > ViewBox[y_max]) ViewBox[y_max] = y;
            if (y < ViewBox[y_min]) ViewBox[y_min] = y;

            std::cout << "    Added worldPoint: " << tempPoint << std::endl;
        }

        polygons.push_back(temp);

    }

    delta_x = ViewBox[x_max] - ViewBox[x_min];
    delta_y = ViewBox[y_max] - ViewBox[y_min];

    delta = std::max(delta_x, delta_y);

    for(Polygon &poly: polygons) {
        for (Point &point: poly) {
            point.xd = (point.x - ViewBox[x_min]) / delta;
            point.yd = (point.y - ViewBox[y_min]) / delta;
        }
    }

    std::cout << " Boundaries: " << ViewBox[x_min] << " " << ViewBox[x_max] << " " << ViewBox[y_min] << " " << ViewBox[y_max] << std::endl;

    input.close();
}

void Polygon::addVertex(Point p) {
    this->push_back(p);
}

std::ostream &operator<< (std::ostream& os, const Point& p) {
    os << "(" << p.xr << "," << p.yr << ")";
    return os;
}


void Polygon::scale(const float &_x,const  float &_y) {
    // Matrix multiplication simplified
    float x_avg = 0, y_avg = 0;
    for(int i = 0; i < size(); i++) {
        x_avg += (*this)[i].xr;
        y_avg += (*this)[i].yr;
    }

    x_avg /= size();
    y_avg /= size();

    std::cout << " Scaling.. " << std::endl;
    for (auto &p: (*this)) {
        std::cout << "    from (" << p.xr << "," << p.yr << ") to ";
        p.xr = (p.xr - x_avg) * _x + x_avg;
        p.yr = (p.yr - y_avg) * _y + y_avg;
        std::cout << "(" << p.xr << "," << p.yr << ")" << std::endl;
    }
}

void Polygon::translate(const float &_x,const  float &_y) {
    // Matrix addition simplified
    std::cout << " Translating.. " << std::endl;
    for (auto &p: (*this)) {
        std::cout << "    from (" << p.xr << "," << p.yr << ") to ";
        p.xr += _x;
        p.yr += _y;
        std::cout << "(" << p.xr << "," << p.yr << ")" << std::endl;
    }
}

void Polygon::rotate(const float &deg_) {

    float deg = deg_ * M_PI / 180;

    std::cout << "Degree is " << deg << std::endl;

    float s = sinf(deg);
    float c = cosf(deg);

    float c_x = 0, c_y = 0;

    for (auto &p: (*this)) {
        c_x += p.xr;
        c_y += p.yr;
    }

    c_x /= size();
    c_y /= size();

    // Matrix multiplication simplified
    std::cout << " Rotating.. " << std::endl;
    for(auto &p: (*this)) {
        std::cout << "    from (" << p.xr << "," << p.yr << ") to ";
        float _x = p.xr, _y = p.yr;
        p.xr = c * _x - s * _y + (c_x - c_x * c + c_y * s);
        p.yr = s * _x + c * _y + (c_y - c_x * s - c_y * c);
        std::cout << "(" << p.xr << "," << p.yr << ")" << std::endl;
    }

}
