COMPILER = gcc
PROGRAM_NAME = shell

all:
	$(COMPILER) main.c -o $(PROGRAM_NAME)

clean:
	rm $(PROGRAM_NAME)
