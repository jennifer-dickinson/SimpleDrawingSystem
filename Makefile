FLAGS = -Wno-deprecated -std=c++11
DEPS = polygon.hpp Draw.hpp helpers.hpp Point.hpp polygon.hpp polyhedron.hpp shadedPolyhedron.hpp
OBJ = polygon.o Lines.o helpers.o DigitalDifferentialAnalyzer.o Bresenham.o Draw.o main.o clipping.o polyhedron.o shadedPolyhedron.o

draw: $(OBJ)
	# g++ -o $@ $(OBJ) $(FLAGS) -lglut -lGLU -lGL ##Linux
	g++ $(FLAGS) -o $@ $(OBJ) -framework GLUT -framework OpenGL ##Mac

%.o : %.cpp $(DEPS)
	g++ $(FLAGS) -c -o $@ $<

clean:
	rm *.o draw
