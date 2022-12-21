CC = clang++
LIBS = -lSDL2 -lSDL2_image

modulation: color_modulation.o
	$(CC) -o modulation color_modulation.o $(LIBS)
	./modulation


color_modulation.o: color_modulation.cpp
	$(CC) -c color_modulation.cpp


clean:
	rm -f modulation color_modulation.o