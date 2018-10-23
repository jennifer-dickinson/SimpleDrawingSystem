//
// Created by Jennifer Salas on 10/2/18.
//

#include "polygon.hpp"
#include "Draw.hpp"

std::vector<Polygon> Draw::initializePolygons(std::string file) {
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

//    float x_min = std::numeric_limits<float>::max(), y_min = std::numeric_limits<float>::max();
//    float x_max = std::numeric_limits<float>::min(), y_max = std::numeric_limits<float>::min();
    
    x_min = std::numeric_limits<float>::max();
    y_min = std::numeric_limits<float>::max();
    x_max = std::numeric_limits<float>::min();
    y_max = std::numeric_limits<float>::min();

    
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
//            if (temp.point.size()) assert(&tempPoint.x != &temp.point.back().x);
            temp.addVertex(tempPoint);
            
            if (x > x_max) x_max = x;
            if (x < x_min) x_min = x;
            if (y > y_max) y_max = y;
            if (y < y_min) y_min = y;
            
            std::cout << "    Added point: " << tempPoint << std::endl;
        }

        polygonSet.push_back(temp);

    }
    
    delta_x = x_max - x_min;
    delta_y = y_max - y_min;
    
    delta = std::max(delta_x, delta_y);
    
    for(Polygon &poly: polygonSet) {
        for (Point &point: poly.point) {
            point.xd = (point.x - x_min) / delta;
            point.yd = (point.y - y_min) / delta;
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
    float x_avg = 0, y_avg = 0;
    for(int i = 0; i < point.size(); i++) {
        x_avg += point[i].xr;
        y_avg += point[i].yr;
    }

    x_avg /= point.size();
    y_avg /= point.size();
    
    std::cout << " Scaling.. " << std::endl;
    for (auto &p: point) {
        std::cout << "    from (" << p.xr << "," << p.yr << ") to ";
        p.xr = (p.xr - x_avg) * _x + x_avg;
        p.yr = (p.yr - y_avg) * _y + y_avg;
        std::cout << "(" << p.xr << "," << p.yr << ")" << std::endl;
    }
}

void Polygon::translate(const float &_x,const  float &_y) {
    // Matrix addition simplified
    std::cout << " Translating.. " << std::endl;
    for (auto &p: point) {
        std::cout << "    from (" << p.xr << "," << p.yr << ") to ";
        p.xr += _x;
        p.yr += _y;
        std::cout << "(" << p.xr << "," << p.yr << ")" << std::endl;
    }
}

void Polygon::rotate(const float &deg) {

    float s = sin(deg);
    float c = cos(deg);
    
    float c_x = 0, c_y = 0;
    
    for (auto &p: point) {
        c_x += p.xr;
        c_y += p.yr;
    }
    
    c_x /= point.size();
    c_y /= point.size();
    
    // Matrix multiplication simplified
    std::cout << " Rotating.. " << std::endl;
    for(auto &p: point) {
        std::cout << "    from (" << p.xr << "," << p.yr << ") to ";
        float _x = p.xr, _y = p.yr;
        p.xr = c * _x - s * _y + (c_x - c_x * c + c_y * s);
        p.yr = s * _x + c * _y + (c_y - c_x * s - c_y * c);
        std::cout << "(" << p.xr << "," << p.yr << ")" << std::endl;
    }

}
