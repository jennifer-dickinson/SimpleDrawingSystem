FLAGS = -Wno-deprecated -std=gnu++11
DEPS = polygon.hpp Draw.hpp Comm.hpp
OBJ = polygon.o Lines.o helpers.o DigitalDifferentialAnalyzer.o Bresenham.o Draw.o main.o clipping.o polyhedron.o shadedPolyhedron.o

draw: $(OBJ)
	# g++ -o $@ $(OBJ) $(FLAGS) -lglut -lGLU -lGL ##Linux
	g++ -o $@ $(OBJ) $(FLAGS) -framework GLUT -framework OpenGL ##Mac

%.o : %.cpp $(DEPS)
	g++ -c -o $@ $<  $(FLAGS)

clean:
	rm *.o draw
