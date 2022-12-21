CC = clang++
LIBS = -lSDL2 -lSDL2_image

sheet: sprite_sheets.o
	$(CC) -o sheet sprite_sheets.o $(LIBS)
	./sheet

sprite_sheets.o: sprite_sheets.cpp
	$(CC) -c sprite_sheets.cpp 

clean:
	rm -f sheet sprite_sheets.o