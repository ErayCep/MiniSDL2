CC = clang++
LIBS = -lSDL2 -lSDL2_image

geometries: geometry.o
	$(CC) -o geometries geometry.o $(LIBS)
	./geometries


geometry.o: geometry.cpp
	$(CC) -c geometry.cpp


clean:
	rm -f geometries geometry.o