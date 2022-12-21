CC = clang++
LIBS = -lSDL2 -lSDL2_image

main: animation.o
	$(CC) -o main animation.o $(LIBS)
	./main

animation.o: animation.cpp
	$(CC) -c animation.cpp


clean:
	rm -f main animation.o