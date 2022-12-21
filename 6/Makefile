CC = clang++
LIBS = -lSDL2 -lSDL2_image

load_imagee: load_image.o
	$(CC) -o load_image load_image.o $(LIBS)
	./load_image


load_image.o: load_image.cpp
	$(CC) -c load_image.cpp


clean:
	rm -f load_image load_image.o