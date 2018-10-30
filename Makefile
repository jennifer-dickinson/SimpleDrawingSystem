FLAGS = -Wno-deprecated -std=c++11
DEPS = polygon.hpp Draw.hpp Comm.hpp
OBJ = polygon.o Lines.o DigitalDifferentialAnalyzer.o Bresenham.o Draw.o main.o clipping.o polyhedron.o


draw: $(OBJ)
	g++ -o $@ $(OBJ) $(FLAGS) -lglut -lGLU -lGL
	# g++ -o $@ $(OBJ) $(FLAGS) -framework GLUT -framework OpenGL 

%.o : %.cpp $(DEPS)
	g++ -c -o $@ $<  $(FLAGS)

clean:
	rm *.o draw
