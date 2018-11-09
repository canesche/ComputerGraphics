LIBS=  -lGL -lGLU -lglut -lm -lSDL2 -lSDL2_mixer

CC=g++

a.out: main.cpp glm.cpp tgaload.cpp
	$(CC)  glm.cpp tgaload.cpp -w -o  $@ $< $(LIBS)

clean: 
	$(RM) *.exe *.o *.out

