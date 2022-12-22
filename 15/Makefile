CC = clang++
LIBS = -lSDL2 -lSDL2_image

rotation: rotation.o
	$(CC) -o rotation rotation.o $(LIBS)
	./rotation

rotation.o: rotation.cpp
	$(CC) -c rotation.cpp

clean:
	rm -f rotation rotation.o