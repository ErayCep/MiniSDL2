CC = clang++
LIBS = -lSDL2 -lSDL2_image

animation: animation.o
	$(CC) -o animation animation.o $(LIBS)
	./animation

animation.o: animation.cpp
	$(CC) -c animation.cpp


clean:
	rm -f animation animation.o