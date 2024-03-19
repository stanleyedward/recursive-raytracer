CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
CFLAGS = -g -DGL_GLEXT_PROTOTYPES -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED -DOSX -Wno-deprecated-register -Wno-deprecated-declarations -Wno-shift-op-parentheses
INCFLAGS = -I./glm-0.9.7.1 -I/usr/X11/include -I./include/
LDFLAGS = -framework GLUT -framework OpenGL -L./lib/mac/ \
		-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
		-lGL -lGLU -lm -lstdc++ -lfreeimage
else
CFLAGS = -g -DGL_GLEXT_PROTOTYPES -no-pie
INCFLAGS = -I./glm-0.9.7.1 -I./include/ -I/usr/X11R6/include -I/sw/include \
		-I/usr/sww/include -I/usr/sww/pkg/Mesa/include
LDFLAGS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib -L./lib/nix/ \
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lGLEW -lglut -lGLU -lGL -lX11 -lfreeimage
endif

all: raytracer

Camera.o : Camera.cpp Camera.h
	${CC} ${CFLAGS} ${INCFLAGS} -c Camera.cpp

Object.o : Object.cpp Object.h
	${CC} ${CFLAGS} ${INCFLAGS} -c Object.cpp

Transform.o: Transform.cpp Transform.h
	${CC} ${CFLAGS}  ${INCFLAGS} -c Transform.cpp

main.o: main.cpp ray_proj.h Transform.h intersection.h readfile.h variables.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
ray_proj.o: ray_proj.cpp ray_proj.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c ray_proj.cpp
readfile.o: readfile.cpp readfile.h variables.h Transform.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c readfile.cpp
intersection.o: intersection.cpp intersection.h ray_proj.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c intersection.cpp


raytracer: main.o intersection.o Transform.o readfile.o ray_proj.o variables.h readfile.h intersection.h Transform.h ray_proj.h 
	$(CC) $(CFLAGS) -o raytracer ray_proj.o main.o Transform.o intersection.o readfile.o $(INCFLAGS) $(LDFLAGS) 

clean: 
	rm -rf *o *~ raytracer