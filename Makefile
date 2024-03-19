CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
CFLAGS = -g -DGL_GLEXT_PROTOTYPES -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED -DOSX -Wno-deprecated-register -Wno-deprecated-declarations -Wno-shift-op-parentheses
INCFLAGS = -I./glm-0.9.7.1 -I/usr/X11/include -I./include/
LDFLAGS = -framework GLUT -framework OpenGL -L./lib/mac/ \
		-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
		-lGL -lGLU -lm -lstdc++ -lfreeimage
else
CFLAGS = -g -DGL_GLEXT_PROTOTYPES 
INCFLAGS = -I./glm-0.9.7.1 -I./include/ -I/usr/X11R6/include -I/sw/include \
		-I/usr/sww/include -I/usr/sww/pkg/Mesa/include
LDFLAGS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib -L./lib/nix/ \
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lGLEW -lglut -lGLU -lGL -lX11 -lfreeimage
endif
  
all : raytracer

Camera.o : Camera.cpp Camera.h
	${CC} ${CFLAGS} ${INCFLAGS} -c Camera.cpp

Transform.o: Transform.cpp Transform.h
	${CC} ${CFLAGS}  ${INCFLAGS} -c Transform.cpp

FileReader.o : FileReader.cpp 
	${CC} ${CFLAGS} ${INCFLAGS} -c FileReader.cpp

Light.o : Light.cpp Light.h 
	${CC} ${CFLAGS} ${INCFLAGS} -c Light.cpp

main.o : main.cpp Scene.h Camera.h FileReader.h Renderer.h
	${CC} ${CFLAGS} ${INCFLAGS} -c main.cpp

Object.o : Object.cpp Object.h
	${CC} ${CFLAGS} ${INCFLAGS} -c Object.cpp

Ray.o : Ray.cpp Ray.h
	${CC} ${CFLAGS} ${INCFLAGS} -c Ray.cpp

Renderer.o : Renderer.cpp Renderer.h
	${CC} ${CFLAGS} ${INCFLAGS} -c Renderer.cpp

Scene.o : Scene.cpp Scene.h
	${CC} ${CFLAGS} ${INCFLAGS} -c Scene.cpp
	
raytracer : main.o Transform.o Camera.o Scene.o Renderer.o Ray.o Object.o Light.o FileReader.o
	${CC} ${CFLAGS} ${INCFLAGS} main.o Transform.o Camera.o Scene.o Renderer.o Ray.o Object.o Light.o FileReader.o ${LDFLAGS} -o raytracer

clean:
	  rm -rf *o *~ raytracer