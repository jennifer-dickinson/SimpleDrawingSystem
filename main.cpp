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

void Menu(Draw &scene);
void Menu3D(Draw &scene);
void MenuShader(Draw &scene);

using namespace std;
bool firstTime = true;


int x = 600;
int y = 600;

Draw scene(x,y, false);

void display();
void allmenu();
int main(int argc, char *argv[]) {
    bool threeDimensional = false, shader = false;
    string file;
    for(int i = 0; i < argc; i++) std::cout << argv[i];
    std::cout << std::endl;
    if (argc == 1) {
        NEEDRENDERTYPE:
        cout << "Please enter the type of rendering:\n" \
            << "\t2d"       << "\tRender two dimensional polygons\n" \
            << "\t3d"       << "\tRender skeletons of polyhedrons\n" \
            << "\tshader"   << "\tRender polyhedrons with shading" \
            << std::endl;
        cin >> file;
    }
    else {
        file = argv[1];
    }
        if (file == "2d") {
            std::cout << "Rendering 2D polygons" << std::endl;
        }
        else if (file == "3d") {
            threeDimensional = true;
            std::cout << "Rendering 3D polyhedron skeletons" << std::endl;
        }
        else if (file == "shader") {
            shader = true;
            std::cout << "Rendering 3D shaded polyhedrons" << std::endl;        }
        else {
            std::cout << file << " is not a valid render type" <<std::endl;
            goto NEEDRENDERTYPE;
        }
        cout << "Please enter a filename: ";
        cin >> file;
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
    std::thread console(allmenu);
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
            Menu(scene);
        }
    }
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
    float x_m, y_m, z_m, x_m2, y_m2, z_m2, degree;

    if (input == "help") {
        std::cout << "    COMMAND     PARAMETERS" << std::endl;
        std::cout << "    --------------------------------------------------" << std::endl;
        std::cout << "    translate   <POLYGON ID> <X MODIFIER> <Y MODIFER> <Z Modifier>" <<std::endl;
        std::cout << "    rotate      <POLYGON ID> <DEGREES> <P1 X> <P1 Y> <P1 Z> <P2 X> <P2 Y> <P2 Z>" << std:: endl;
        std::cout << "    scale       <POLYGON ID> <FACTOR>" <<std::endl;
        std::cout << "    view        <XY | XZ | YZ | CAVALIER | CABINET>" << std::endl;
        // std::cout << "    info        <POLYHEDRON ID>" << std::endl;
        std::cout << "    save        <FILENAME>" << std::endl;
        std::cout << "    exit" << std::endl;
    } else if (input == "scale") {
        cin >> id >> x_m;
        scene.undoOblique();
        scene.polyhedrons[id].scale(x_m);
    } else if (input == "translate") {
        cin >> id >> x_m >> y_m >> z_m;
        scene.undoOblique();
        scene.polyhedrons[id].translate(Point3D(x_m, y_m, z_m));
    } else if (input == "rotate") {
        cin >> id >> degree >> x_m >> y_m >> z_m >> x_m2 >> y_m2 >> z_m2;
        scene.undoOblique();
        scene.polyhedrons[id].rotate(degree, Point3D(x_m, y_m, z_m), Point3D(x_m2, y_m2, z_m2));

    } else if (input == "view") {
        cin >> input;
        if (input == "XY" || input == "xy") {
            scene.undoOblique();
            scene.viewXY();
        }
        else if (input == "XZ" || input == "xz") {
            scene.undoOblique();
            scene.viewXZ();
        }
        else if (input == "YZ" || input == "yz") {
            scene.undoOblique();
            scene.viewYZ();
        }
        else if (input == "CAVALIER" || input == "cavalier")  {
            scene.undoOblique();
            scene.viewCavalier();
        }
        else if (input == "CABINET" || input == "cabinet") {
            scene.undoOblique();
            scene.viewCabinet();
        }
        else std::cout << input << " is an invalid projection." << std::endl;

    } else if (input == "save") {
        cin >> input;
        scene.save3D(input);

    } else if (input == "info") {

    } else if (input == "exit") {
        std::cout << "Program is now exiting." << std::endl;
        exit(0);
    }
    else {
        cout << "That is not a valid action." << std::endl;
        std::getline(std::cin, input); // Used to flush out the rest of the commands
    }
    scene.normalize();
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
    } else if (input == "rasterize") {
        cin >> input;
        if (input == "true" || input == "t")scene.raster = true;
        else if (input == "false" || input == "f")scene.raster = false;
        else scene.raster = !scene.raster;
        std::cout << "Rasterize set to " << (scene.raster? "true": "false") << std::endl;
    }
    else {
        cout << "That is not a valid action." << std::endl;
        std::getline(std::cin, input); // Used to flush out the rest of the commands
    }
}

void MenuShader(Draw &scene) {
    string input;

    cout <<  std::endl << "Enter action (type help for available commands): " << flush;
    cin >> input;
    int id = 0;
    float x_m, y_m, z_m, x_m2, y_m2, z_m2, degree;

    if (input == "help") {
        std::cout << "    COMMAND     PARAMETERS" << std::endl;
        std::cout << "    --------------------------------------------------" << std::endl;
        // std::cout << "    translate   <POLYGON ID> <X MODIFIER> <Y MODIFER> <Z Modifier>" <<std::endl;
        // std::cout << "    rotate      <POLYGON ID> <DEGREES> <P1 X> <P1 Y> <P1 Z> <P2 X> <P2 Y> <P2 Z>" << std:: endl;
        // std::cout << "    scale       <POLYGON ID> <FACTOR>" <<std::endl;
        std::cout << "    view        <XY | XZ | YZ >" << std::endl;
        std::cout << "    info        <POLYHEDRON ID>" << std::endl;
        // std::cout << "    save        <FILENAME>" << std::endl;
        std::cout << "    exit" << std::endl;
    // } else if (input == "scale") {
    //     cin >> id >> x_m;
    //     scene.polyhedrons[id].scale(x_m);
    // } else if (input == "translate") {
    //     cin >> id >> x_m >> y_m >> z_m;
    //     scene.polyhedrons[id].translate({x_m, y_m, z_m});
    // } else if (input == "rotate") {
    //     cin >> id >> degree >> x_m >> y_m >> z_m >> x_m2 >> y_m2 >> z_m2;
    //     scene.polyhedrons[id].rotate(degree, {x_m, y_m, z_m}, {x_m2, y_m2, z_m2});

    } else if (input == "view") {
        cin >> input;
        if (input == "XY" || input == "xy") {
            scene.viewXY();
        }
        else if (input == "XZ" || input == "xz") {
            scene.undoOblique();
            scene.viewXZ();
        }
        else if (input == "YZ" || input == "yz") {
            scene.viewYZ();
        } else std::cout << input << " is an invalid projection." << std::endl;

    // } else if (input == "save") {
    //     cin >> input;
    //     scene.savePolyhedrons(input);

    // } else if (input == "info") {

    } else if (input == "exit") {
        std::cout << "Program is now exiting." << std::endl;
        exit(0);
    }
    else {
        cout << "That is not a valid action." << std::endl;
        std::getline(std::cin, input); // Used to flush out the rest of the commands
    }
    // scene.normalize();
}
