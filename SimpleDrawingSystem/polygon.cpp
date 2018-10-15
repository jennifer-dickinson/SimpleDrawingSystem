//
// Created by Jennifer Salas on 10/2/18.
//

#include "polygon.hpp"

std::vector <Polygon> initializePolygons(std::string file) {
    std::vector <Polygon> polygonSet;
    std::ifstream input(file);
    std::string line;

    if (!input) {
        std::cout << "Could not find file: " << file << std::endl;
        std::exit(0);
    }

    int numPolygons = 0, vertices = 0;
    float x = 0.0, y = 0.0;

    std::getline(input, line);
    std::cout << line << std::endl;
    std::istringstream ss(line);

    ss >> numPolygons;

    std::cout << "Total polygons: " << numPolygons << std::endl;

    float x_min = std::numeric_limits<float>::max(), y_min = std::numeric_limits<float>::max();
    float x_max = std::numeric_limits<float>::min(), y_max = std::numeric_limits<float>::min();
    
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
            if (temp.point.size()) assert(&tempPoint.x != &temp.point.back().x);
            temp.addVertex(tempPoint);
            
            if (x > x_max) x_max = x;
            if (x < x_min) x_min = x;
            if (y > y_max) y_max = y;
            if (y < y_min) y_min = y;
        }

        polygonSet.push_back(temp);

    }
    
    float delta_x = x_max - x_min , delta_y = y_max - y_min;
    
    float delta = std::max(delta_x, delta_y);
    
    for(Polygon &poly: polygonSet) {
        for (Point &point: poly.point) {
            point.xd = (point.x - x_min) / delta;
            point.yd = (point.y - y_min) / delta;
            std::cout << point <<  "=>" << "(" << point.xd << "," << point.yd << ")" << std::endl;
        }
    }

    input.close();

    return polygonSet;
}

void Polygon::addVertex(Point p) {
    this->point.push_back(p);
    this->vertices++;
}

std::ostream &operator<< (std::ostream& os, const Point& p) {
    os << "(" << p.x << "," << p.y << ")";
    return os;
}


void Polygon::scale(const float &_x,const  float &_y) {
    // Matrix multiplication simplified
    for (auto &p: point) {
        p.x *= _x;
        p.y *= _y;
    }
}

void Polygon::translate(const float &_x,const  float &_y) {
    // Matrix addition simplified
    for (auto &p: point) {
        p.x += _x;
        p.y += _y;
    }
}

void Polygon::rotate(const float &deg) {
    float x_avg = 0, y_avg = 0;
    
    // Get the average locaiton of the polygon
    for(auto &p: point) {
        x_avg = p.x;
        y_avg = p.y;
    }
    
    x_avg /= point.size();
    y_avg /= point.size();
    
    // Move to origin
    translate(-x_avg, -y_avg);
    
    // rotate - matrix multiplication simplified
    
    for(auto &p: point) {
        float _x = p.x;
        p.x = _x * cos(deg) - p.y * sin(deg);
        p.y = _x * sin(deg) + p.y * cos(deg);
    }
    
    // Return to original position
    translate(x_avg, y_avg);
}
