//
//  main.cpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/9/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

// Compile: g++ -o draw polygon.cpp Draw.cpp main.cpp -framework GLUT -framework OpenGL -lcurses


//*** CSIF Specific ***//
// #include <GL/glut.h>
//*** End CSIF Specific ***//

//*** Mac Specific ***//
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
//*** End Mac Specific ***//

#include "Draw.hpp"

#include <iostream>
#include <thread>
#include <cstdlib>

void Menu2D(Draw &scene);
void Menu3D(Draw &scene);
void MenuShader(Draw &scene);
std::vector<std::string> split(std::string input, char delim = ' ');

using namespace std;
bool firstTime = true;


int x = 600;
int y = 600;

Draw scene(x,y, false);

void display();
void allmenu();
int main(int argc, char *argv[]) {
    bool threeDimensional = false, shader = false;
    std::string file;
    for(int i = 0; i < argc; i++) cout << argv[i];
    cout << endl;
    if (argc == 1) {
        NEEDRENDERTYPE:
        cout << "Please enter the type of rendering:\n" \
            << "\t2d"       << "\tRender two dimensional polygons\n" \
            << "\t3d"       << "\tRender skeletons of polyhedrons\n" \
            << "\tshader"   << "\tRender polyhedrons with shading\n" \
            << "> ";
        getline(cin, file);
    }
    else {
        file = argv[1];
    }
        if (file == "2d") {
            cout << "Rendering 2D polygons" << endl;
        }
        else if (file == "3d") {
            threeDimensional = true;
            cout << "Rendering 3D polyhedron skeletons" << endl;
        }
        else if (file == "shader") {
            shader = true;
            cout << "Rendering 3D shaded polyhedrons" << endl;        }
        else {
            cout << file << " is not a valid render type" <<endl;
            goto NEEDRENDERTYPE;
        }
        cout << "Please enter a filename: \n> ";
        getline(cin, file);
    // }

    scene.initialize(file, threeDimensional, shader);


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    //set window size to 200*200
    glutInitWindowSize(x, y);
    //set window position
    glutInitWindowPosition(100, 100);

    //create and set main window title
    glutCreateWindow("Simple Drawing System");
    glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL
    //sets display function
    thread console(allmenu);
    glutDisplayFunc(display);
    glutMainLoop();//main display loop, will display until terminate

    return 0;
}

void allmenu() {
    while(true) {
        if (firstTime) {
            firstTime = false;
        }
        else if (scene.Shader) {
            MenuShader(scene);
        }
        else if(scene.ThreeDimensional) {
            Menu3D(scene);
        } else {
            Menu2D(scene);
        }
    }
}

//main display loop, this function will be called again and again by OpenGL
void display()
{
    // allmenu();
    //Misc.
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //draws pixel on screen, width and height must match pixel buffer dimension
    glDrawPixels(x, y, GL_RGB, GL_FLOAT, scene);

    //window refresh
    glFlush();

    glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene.draw();

    glutPostRedisplay();

}

// 1 2 3 4 5

std::vector<std::string> split(std::string input, char delim) {
    std::vector<std::string> substrs;
    for(int i = 0; i < input.size(); i++) {
        while(i < input.size() && input[i] == delim) i++;
        int j = i;
        while(j < input.size() && input[j] != delim) j++;
        substrs.push_back(input.substr(i,j - i));
        i = j;
    }
    return substrs;
}

void Menu3D(Draw &scene) {
    std::string input;

    cout <<  endl << "Enter action (type help for available commands): " << flush;
    getline(cin, input);
    std::vector<std::string> args(split(input));

    int id = 0;
    float x_m, y_m, z_m, x_m2, y_m2, z_m2, degree;

    if (args.size() == 1 && args[0] == "help") {
        cout << "    COMMAND     PARAMETERS" << endl;
        cout << "    --------------------------------------------------" << endl;
        cout << "    translate   <POLYGON ID> <X MODIFIER> <Y MODIFER> <Z Modifier>" <<endl;
        cout << "    rotate      <POLYGON ID> <DEGREES> <P1 X> <P1 Y> <P1 Z> <P2 X> <P2 Y> <P2 Z>" <<  endl;
        cout << "    scale       <POLYGON ID> <FACTOR>" <<endl;
        cout << "    view        <XY | XZ | YZ | CAVALIER | CABINET>" << endl;
        // cout << "    info        <POLYHEDRON ID>" << endl;
        cout << "    save        <FILENAME>" << endl;
        cout << "    exit" << endl;
    } else if (args.size() == 3 && args[0] == "scale") {
        try {
            id = stoi(args[1]);
            x_m = stof(args[2]);
        } catch (const invalid_argument &e) {
            cout << "Invalid arguments for scale." << endl;
            return;
        }
        scene.undoOblique();
        scene.polyhedrons[id].scale(x_m);
    } else if (args.size() == 5 && args[0] == "translate") {
        try {
            id = stoi(args[1]);
            x_m = stof(args[2]);
            y_m= stof(args[3]);
            z_m = stof(args[4]);
        } catch (const invalid_argument &e) {
            cout << "Invalid arguments for translate." << endl;
            return;
        }
        scene.undoOblique();
        scene.polyhedrons[id].translate(Point3D(x_m, y_m, z_m));
    } else if (args.size() == 9 && args[0] == "rotate") {
        try {
            id = stoi(args[1]);
            degree = stof(args[2]);
            x_m = stof(args[3]);
            y_m = stof(args[4]);
            z_m = stof(args[5]);
            x_m2 = stof(args[6]);
            y_m2 = stof(args[7]);
            z_m2 = stof(args[8]);
        } catch (const invalid_argument &e) {
            cout << "Invalid arguments for rotate." << endl;
            return;
        }
        scene.undoOblique();
        scene.polyhedrons[id].rotate(degree, Point3D(x_m, y_m, z_m), Point3D(x_m2, y_m2, z_m2));

    } else if (args.size() == 2 && args[0] == "view") {
        if (args[1] == "XY" || args[1] == "xy") {
            scene.undoOblique();
            scene.viewXY();
        }
        else if (args[1] == "XZ" || args[1] == "xz") {
            scene.undoOblique();
            scene.viewXZ();
        }
        else if (args[1] == "YZ" || args[1] == "yz") {
            scene.undoOblique();
            scene.viewYZ();
        }
        else if (args[1] == "CAVALIER" || args[1] == "cavalier")  {
            scene.undoOblique();
            scene.viewCavalier();
        }
        else if (args[1] == "CABINET" || args[1] == "cabinet") {
            scene.undoOblique();
            scene.viewCabinet();
        }
        else cout << args[1] << " is an invalid projection." << endl;

    } else if (args.size() == 2 && args[0] == "save") {
        scene.save3D(args[1]);

    } else if (args.size() == 1 && args[0] == "info") {

    } else if (args.size() == 1 && args[0] == "exit") {
        cout << "Program is now exiting." << endl;
        exit(0);
    }
    else {
        cout << "That is not a valid action." << endl;
        return;
    }
    scene.normalize();
}

void Menu2D(Draw &scene) {
    std::string input;

    cout <<  endl << "Enter action (type help for available commands): " << flush;
    getline(cin, input);
    std::vector<std::string> args(split(input));
    int id = 0;
    float x_m, y_m;

    if (args.size() >= 1 && args[0] == "help") {
        cout << "    COMMAND     PARAMETERS" << endl;
        cout << "    --------------------------------------------------" << endl;
        cout << "    algorithm   <bresenham | dda>" << endl;
        cout << "    translate   <POLYGON ID> <X MODIFIER> <Y MODIFER>" <<endl;
        cout << "    rotate      <POLYGON ID> <DEGREES>" <<  endl;
        cout << "    scale       <POLYGON ID> <X MODIFIER> <Y MODIFER>" <<endl;
        cout << "    viewport    <X LOWER> <X UPPER> <Y LOWER> <Y UPPER>" << endl;
        cout << "    info        <POLYGON ID>" << endl;
        cout << "    save        <FILENAME>" << endl;
        cout << "    exit" << endl;

    }
    else if (args.size() == 2 && args[0] == "algorithm") {
        if (args[1] == "bresenham") scene.bresenhamAlgo = true;
        else if (args[1] == "dda") scene.bresenhamAlgo = false;
        else {
            cout << "That is not a valid algorithm" << endl;
            return;
        }
        cout << "Line rendering algoirthm set to " << args[1] << endl;
    }
    else if (args.size() == 4 && args[0] == "scale") {
        try {
            id = stoi(args[1]);
            x_m = stof(args[2]);
            y_m= stof(args[3]);
            scene[id].scale(x_m, y_m);
        } catch (const invalid_argument &e) {
            cout << "Invalid arguments for scale." << endl;
            return;
        }
    } else if (args.size() == 4 && args[0] == "translate") {
        try{
            id = stoi(args[1]);
            x_m = stof(args[2]);
            y_m= stof(args[3]);
            scene[id].translate(x_m, y_m);
        } catch (const invalid_argument &e) {
            cout << "Invalid arguments for translate." << endl;
            return;
        }
    } else if (args.size() == 3 && args[0] == "rotate") {
        try {
            id = stoi(args[1]);
            x_m = stof(args[2]);
            scene[id].rotate(x_m);
        } catch (const invalid_argument &e) {
            cout << "Invalid arguments for rotate" << endl;
            return;
        }
    // } else if (args.size() == 1 && args[0] == "viewport") {
    } else if (args.size() >= 2 && args[0] == "save") {
        scene.save(args[1]);
        cout << "Modified polygons saved to file " << args[1] << endl;
    } else if (args.size() == 2 && args[0] == "info") {
        try {
            id = stoi(args[1]);
        } catch (const invalid_argument &e) {
            cout << "Invalid arguments for info" << endl;
            return;
        }
        scene.info(id);
    } else if (args.size() == 1 && args[0] == "exit") {
        cout << "Program is now exiting." << endl;
        exit(0);
    } else if (args.size() == 2 && args[0] == "rasterize") {
        if (args[1] == "true" || args[1] == "t") scene.raster = true;
        else if (args[1] == "false" || args[1] == "f") scene.raster = false;
        else { cout << "Invalid arguments for rasterize" << endl; return;}
        cout << "Rasterize set to " << (scene.raster? "true": "false") << endl;
    } else if (args.size() == 1 && args[0] == "rasterize") {
        scene.raster = !scene.raster;
        cout << "Rasterize switched to " << (scene.raster? "true": "false") << endl;
    }
    else {
        cout << "That is not a valid action." << endl;
    }
}

void MenuShader(Draw &scene) {
    std::string input;

    cout <<  endl << "Enter action (type help for available commands): " << flush;
    getline(cin, input);
    std::vector<std::string> args(split(input));
    int id = 0;
    float x_m, y_m, z_m, x_m2, y_m2, z_m2, degree;

    if (args.size() == 1 && args[0] == "help") {
        cout << "    COMMAND     PARAMETERS" << endl;
        cout << "    --------------------------------------------------" << endl;
        // cout << "    translate   <POLYGON ID> <X MODIFIER> <Y MODIFER> <Z Modifier>" <<endl;
        // cout << "    rotate      <POLYGON ID> <DEGREES> <P1 X> <P1 Y> <P1 Z> <P2 X> <P2 Y> <P2 Z>" <<  endl;
        // cout << "    scale       <POLYGON ID> <FACTOR>" <<endl;
        cout << "    view        <XY | XZ | YZ >" << endl;
        cout << "    info        <POLYHEDRON ID>" << endl;
        // cout << "    save        <FILENAME>" << endl;
        cout << "    exit" << endl;

    } else if (args.size() == 2 && args[0] == "view") {
        if (args[1] == "XY" || args[1] == "xy") {
            scene.viewXY();
        }
        else if (args[1] == "XZ" || args[1] == "xz") {
            scene.undoOblique();
            scene.viewXZ();
        }
        else if (args[1] == "YZ" || args[1] == "yz") {
            scene.viewYZ();
        } else cout << args[1] << " is an invalid projection." << endl;

    } else if (args.size() == 1 && args[0] == "exit") {
        cout << "Program is now exiting." << endl;
        exit(0);
    }
    else {
        cout << "That is not a valid action." << endl;
    }
    // scene.normalize();
}
