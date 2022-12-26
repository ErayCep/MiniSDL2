CC = clang++
LIBS = -lSDL2 -lSDL2_image

main: key_events.o
	$(CC) -o main key_events.o $(LIBS)
	./main


key_events.o: key_events.cpp
	$(CC) -c key_events.cpp


clean:
	rm -f main key_events.o