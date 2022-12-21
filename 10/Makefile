CC = clang++
LIBS = -lSDL2 -lSDL2_image

keying: color_keying.o
	$(CC) -o keying color_keying.o $(LIBS)
	./keying


color_keying.o: color_keying.cpp
	$(CC) -c color_keying.cpp


clean:
	rm -f keying color_keying.o