//
//  main.cpp
//  SimpleDrawingSystem
//
//  Created by Jennifer Salas on 10/9/18.
//  Copyright © 2018 Jennifer Salas. All rights reserved.
//

// Compile: g++ -o draw polygon.cpp Draw.cpp main.cpp -framework GLUT -framework OpenGL -lcurses
/*
 Mac Specific Headers below
 Use #include <GL/glut.h> on csif
 */
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "Draw.hpp"
#include "Comm.hpp"

#include <iostream>

using namespace std;


int x = 640;
int y = 480;

Draw scene(x,y, true);

float *PixelBuffer;
void display();
int main(int argc, char *argv[]) {
    string file("");
    if (argc == 2) {
        file = argv[1];
    }
    else if (argc == 1){
        cout << "Please enter a filename: ";
        cin >> file;
    }
    
    vector<Polygon> world = initializePolygons(file);
    
    for(Polygon poly: world) scene.draw(poly);
    
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    //set window size to 200*200
    glutInitWindowSize(x, y);
    //set window position
    glutInitWindowPosition(100, 100);
    
    //create and set main window title
    int MainWindow = glutCreateWindow("Simple Drawing System");
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
    
    string test;
    
    cout << "Enter action (transform, viewport, exit): " << flush;
    cin >> test;
    int id = 0;
    float x_m, y_m;

    if (test == "transform") {
        cout << "Enter the ID of the polygon to be manipulated: " << flush;
        cin >> id;
        cout << "Enter the type of transformation (scale, translate, rotate): " << flush;
        cin >> test;
        
        if (test == "scale") {
            cout << "Enter the x and y scale (e.g. 1.0 1.0): ";
            cin >> x_m >> y_m;
            
        } else if (test == "transform") {
            cout << "Enger the x and y direction to move (e.g. 1.0 1.0): ";
            cin >> x_m >> y_m;
            

        } else if (test == "rotate") {
            cout << "Enter the angle in degrees to rotate (e.g. 45): ";
            cin >> x_m;
            
        } else cout << "That is not a valid transformation." << std::endl;
    } else if (test == "viewport") {
        
    } else if (test == "exit") exit(0);
    else cout << "That is not a valid action." << std::endl;
    
    glutPostRedisplay();

}
