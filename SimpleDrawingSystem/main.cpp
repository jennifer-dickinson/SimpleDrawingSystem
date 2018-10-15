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
#include <sys/types.h>
#include <unistd.h>
#include <curses.h>
#include <poll.h>
#include <sys/types.h>
#include <signal.h>

using namespace std;


int x = 640;
int y = 480;

Draw scene(x,y, true);
pid_t parent = getpid();

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
    
    int pipes[2];
    if(pipe(pipes)) {
        cout << "Failed to create pipe" << endl;
        exit(0);
    }
    
    cout << "Command: " << std::flush;
    
    pid_t pid = fork();
    
    if (pid) {
        dup2(pipes[1], STDOUT_FILENO);
        close(pipes[0]);
        close(pipes[1]);

        string test;
        
        struct pollfd fd_pol[1];
        
        fd_pol[0].fd = STDIN_FILENO;
        fd_pol[0].events = POLLIN;
        fd_pol[0].revents = POLLIN;
        
        while(test != "exit") {
            if (poll(fd_pol, 1, 50)) {
                getline(cin, test);
                if(write(STDOUT_FILENO, (test + '\n').c_str(), test.size() + 1) == -1) {
                    cerr << "Something broke" << endl;
                }
                cerr << "Command: " << std::flush;
            }
        }

        kill(pid,SIGTERM);
        wait(NULL);

        
    } else {
        
        
        dup2(pipes[0], STDIN_FILENO);
        close(pipes[1]);
        close(pipes[0]);
        
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
        

        
    }
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
    
    struct pollfd fd_pol[1];
    
    fd_pol[0].fd = STDIN_FILENO;
    fd_pol[0].events = POLLIN;
    fd_pol[0].revents = POLLIN;
    
    string test;
    
    if (poll(fd_pol, 1, 0)) {
        getline(cin, test);
        menu(test);
    }

}
