COMPILER = gcc
PROGRAM_NAME = shell

all:
	$(COMPILER) shell.c -o $(PROGRAM_NAME)

clean:
	rm $(PROGRAM_NAME)
