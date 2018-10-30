//
//  main.cpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/9/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

// Compile: g++ -o draw polygon.cpp Draw.cpp main.cpp -framework GLUT -framework OpenGL -lcurses

#include <GL/glut.h>  // CSIF Specific

#include "Draw.hpp"

#include <iostream>

void Menu(Draw &scene);
void Menu3D(Draw &scene);

using namespace std;
bool firstTime = true;


int x = 600;
int y = 600;

Draw scene(x,y, false, {});

void display();
int main(int argc, char *argv[]) {
    bool tD;
    string file;
    for(int i = 0; i < argc; i++) std::cout << argv[i];
    std::cout << std::endl;
    if (argc == 2 && std::string("3d") != argv[1]) {
        file = argv[1];
    }
    if (argc == 3 && std::string("3d") == argv[1]) {
        file = argv[2];
        std::cout << file << " 3d!" << std::endl;
        tD = true;
    }
    else{
        cout << "Please enter the type of rendering (2d or 3d): ";
        cin >> file;
        tD = "3d" == file;
        cout << "Please enter a filename: ";
        cin >> file;
    }

    scene.initialize(file, tD);


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
    glutDisplayFunc(display);

    glutMainLoop();//main display loop, will display until terminate

    return 0;
}

//main display loop, this function will be called again and again by OpenGL
void display()
{

    //Misc.
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //draws pixel on screen, width and height must match pixel buffer dimension
    glDrawPixels(x, y, GL_RGB, GL_FLOAT, scene);

    //window refresh
    glFlush();

    if (firstTime) {
        firstTime = false;
    } else if(scene.ThreeDimensional) {
        Menu3D(scene);
    } else {
        Menu(scene);
    }

    glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene.draw();

    glutPostRedisplay();

}

void Menu3D(Draw &scene) {
    string input;

    cout <<  std::endl << "Enter action (type help for available commands): " << flush;
    cin >> input;
    int id = 0;
    float x_m, y_m;

    if (input == "help") {
        std::cout << "    COMMAND     PARAMETERS" << std::endl;
        std::cout << "    --------------------------------------------------" << std::endl;
        std::cout << "    translate   <POLYGON ID> <X MODIFIER> <Y MODIFER> <Z Modifier>" <<std::endl;
        std::cout << "    rotate      <POLYGON ID> <DEGREES>" << std:: endl;
        std::cout << "    scale       <POLYGON ID> <FACTOR>" <<std::endl;
        std::cout << "    projection    <XY | XZ | YZ>" << std::endl;
        std::cout << "    info        <POLYHEDRON ID>" << std::endl;
        std::cout << "    save        <FILENAME>" << std::endl;
        std::cout << "    exit" << std::endl;
    } else if (input == "scale") {

    } else if (input == "translate") {

    } else if (input == "rotate") {

    } else if (input == "projection") {
        cin >> input;
        if (input == "XY") scene.viewXY();
        else if (input == "XZ") scene.viewXZ();
        else if (input ==  "YZ") scene.viewYZ();
        else std::cout << input << " is an invalid projection." << std::endl;

    } else if (input == "save") {

    } else if (input == "info") {

    } else if (input == "exit") {
        std::cout << "Program is now exiting." << std::endl;
        exit(0);
    }
    else {
        cout << "That is not a valid action." << std::endl;
        std::getline(std::cin, input); // Used to flush out the rest of the commands
    }
}

void Menu(Draw &scene) {
    string input;

    cout <<  std::endl << "Enter action (type help for available commands): " << flush;
    cin >> input;
    int id = 0;
    float x_m, y_m;

    if (input == "help") {
        std::cout << "    COMMAND     PARAMETERS" << std::endl;
        std::cout << "    --------------------------------------------------" << std::endl;
        std::cout << "    algorithm   <bresenham | dda>" << std::endl;
        std::cout << "    translate   <POLYGON ID> <X MODIFIER> <Y MODIFER>" <<std::endl;
        std::cout << "    rotate      <POLYGON ID> <DEGREES>" << std:: endl;
        std::cout << "    scale       <POLYGON ID> <X MODIFIER> <Y MODIFER>" <<std::endl;
        std::cout << "    viewport    <X LOWER> <X UPPER> <Y LOWER> <Y UPPER>" << std::endl;
        std::cout << "    info        <POLYGON ID>" << std::endl;
        std::cout << "    save        <FILENAME>" << std::endl;
        std::cout << "    exit" << std::endl;

    }
    else if (input == "algorithm") {
        cin >> input;
        if (input == "bresenham") scene.bresenhamAlgo = true;
        else if (input == "dda") scene.bresenhamAlgo = false;
        else {
            std::cout << "That is not a valid algorithm" << std::endl;
        }
    }
    else if (input == "scale") {
        cin >> id >> x_m >> y_m;
        scene[id].scale(x_m, y_m);
    } else if (input == "translate") {
        cin >> id >>  x_m >> y_m;
        scene[id].translate(x_m, y_m);
    } else if (input == "rotate") {
        cin >> id >>  x_m;
        scene[id].rotate(x_m);
    } else if (input == "viewport") {

    } else if (input == "save") {
        cin >> input;
        scene.save(input);
    } else if (input == "info") {
        cin >> id;
        scene.info(id);
    } else if (input == "exit") {
        std::cout << "Program is now exiting." << std::endl;
        exit(0);
    }
    else {
        cout << "That is not a valid action." << std::endl;
        std::getline(std::cin, input); // Used to flush out the rest of the commands
    }
}
