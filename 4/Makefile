CC = clang++
LIBS = -lSDL2 -lSDL2_image

inputs: key_inputs.o
	$(CC) -o inputs key_inputs.o $(LIBS)
	./inputs

key_inputs.o: key_inputs.cpp
	$(CC) -c key_inputs.cpp


clean:
	rm -f inputs key_inputs.o