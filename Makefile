CXX = g++
LDLIBS = -lGLEW -lGL -lGLU -lglut -lm
ODIR=obj
TARGETS = terrain_generator

DEPS = diamond_square.h main.h osusphere.h glslprogram.h glew.h \
	freeglut_std.h freeglut_ext.h freeglut.h glut.h

_OBJ = diamond_square.o main.o osusphere.o glslprogram.o terrain_generator.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	@mkdir -p $(ODIR)
	$(CXX) -c -o $@ $<

terrain_generator: $(OBJ)
	$(CXX) -o $@ $^ $(LDLIBS)

clean:
	$(RM) $(TARGETS) $(ODIR)/*.o

.PHONY: clean
