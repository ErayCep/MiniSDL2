CC = clang++
LIBS = -lSDL2 -lSDL2_image

viewports: viewport.o
	$(CC) -o viewports viewport.o $(LIBS)
	./viewports


viewport.o: viewport.cpp
	$(CC) -c viewport.cpp


clean:
	rm -f viewports viewport.o