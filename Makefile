CC = gcc
FILES = 3DMath.h 3DMath.c MoakRaster.c MeshLoad.h TriangleMesh.h TriangleMesh.c Material.h Window.h Render.h Render.c

OUT_EXE = MoakRaster

build:	$(FILES)
		$(CC) -o $(OUT_EXE) $(FILES) -lm -L/usr/local/lib -lSDL2

clean:
		rm -f *.o core

rebuild: clean build

