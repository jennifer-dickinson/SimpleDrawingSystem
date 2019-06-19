
# Simple Drawing System

Author: Jennifer Salas <br>
Date: June 18, 2019

## Purpose

The Simple Drawing system is a program to showcase different techniques for
displaying graphical images.


#### Compile and Run

		make
		./draw
		<Render Type> (3DS for project 3);
		<filename> (ShadedPolyhedron provided)


While in the program, type help for commands

  A a few sample files are included. The format for a file is the following:

	3               # of polyhedrons

	4               # of vertices
	3 3 3 1 1 1     # X Y Z R G B
	11 0 8 0 0 1
	8 11 0 0 1 0
	0 8 11 1 0 0
	4               # number of triangles
	0 1 2           # V1 V2 V3
	0 3 1
	0 2 3
	1 3 2

  Sample files

	samplePolyhedron
	samplePolyhedron2
	ShadedPolyhedron
	spoly
