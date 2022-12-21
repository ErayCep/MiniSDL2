CC = clang++
LIBS = -lSDL2 -lSDL2_image

alpha: alpha_blending.o
	$(CC) -o alpha alpha_blending.o $(LIBS)
	./alpha

alpha.o: alpha_blending.cpp
	$(CC) -c alpha_blending.cpp

clean:
	rm -f alpha alpha_blending.o