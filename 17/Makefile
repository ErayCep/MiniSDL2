CC = clang++
LIBS = -lSDL2 -lSDL2_image

main: mouse_events.o
	$(CC) -o main mouse_events.o $(LIBS)
	./main


mouse_events.o: mouse_events.cpp
	$(CC) -c mouse_events.cpp


clean:
	rm -f main mouse_events.o