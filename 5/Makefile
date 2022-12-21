CC = clang++
LIBS = -lSDL2 -lSDL2_image

optimize: keys_optimize.o
	$(CC) -o optimize keys_optimize.o $(LIBS)
	./optimized

keys_optimize.o: keys_optimize.cpp
	$(CC) -c keys_optimize.cpp


clean:
	rm -f optimize keys_optimize.o