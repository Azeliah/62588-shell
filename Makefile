COMPILER = gcc
PROGRAM_NAME = shell

all:
	$(COMPILER) -o $(PROGRAM_NAME) shell.c

release:
	$(COMPILER) -O3 -o $(PROGRAM_NAME) shell.c

clean:
	rm $(PROGRAM_NAME)
