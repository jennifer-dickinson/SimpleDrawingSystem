//
//  Comm.cpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/14/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

#include "Comm.hpp"

void menu(std::string line) {
    std::vector<std::string> token;
    int start, stop;
    for(start = 0; start < line.size(); start++) {
        for(stop = start; stop < line.size(); stop++)
            if (line[stop] == ' ') break;
        token.push_back(line.substr(start, (stop - start)));
        start = stop;
    }
    
    for (auto tok: token) std::cout << tok << std::endl;
    
    if (token.size() > 1) {
        if (token[0] == "rotate") {
            return;
        } else if(token[0] == "translate") {
            return;
        } else if (token[0] == "scale") {
            return;
        }
    }
    
    std::cout << "Invalid command" << std::endl;
    
}
