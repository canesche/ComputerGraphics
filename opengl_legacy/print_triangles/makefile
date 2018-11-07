# Linux (default)
EXE = main
LDFLAGS = -lGL -lGLU -lglut

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXE = main.exe
	LDFLAGS = -lopengl32 -lglu32 -lglut32
endif

# OS X
ifeq "$(OSTYPE)" "darwin"
	LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
endif

$(EXE) : main.cpp
	gcc -o $@ $< $(CFLAGS) $(LDFLAGS)
