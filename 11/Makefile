CC = clang++
LIBS = -lSDL2 -lSDL2_image

sheets: sprite_sheets.o
	$(CC) -o sheets sprite_sheets.o $(LIBS)
	./sheets

sprite_sheets.o: sprite_sheets.cpp
	$(CC) -c sprite_sheets.cpp 

clean:
	rm -f sheets sprite_sheets.o