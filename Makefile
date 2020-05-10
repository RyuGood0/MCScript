exec = mcs.exe
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g

CC = gcc

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

install:
	make

clean:
	-rm mcs.exe
	-rm *.o
	-rm src/*.o