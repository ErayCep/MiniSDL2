CC = clang++
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf

main: fonts.o
	$(CC) -o main fonts.o $(LIBS)
	./main

fonts.o: fonts.cpp
	$(CC) -c fonts.cpp


clean:
	rm -f fonts.o main