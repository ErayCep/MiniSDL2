CC = clang++
LIBS = -lSDL2 -lSDL2_image

textures: textures.o
	$(CC) -o textures textures.o $(LIBS)
	./textures


textures.o: textures.cpp
	$(CC) -c textures.cpp


clean:
	rm -f textures textures.o