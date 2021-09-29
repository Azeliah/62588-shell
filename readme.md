# 62588-shell

## Requirements
- CMake
- GNU Make

### How to install
In bash, type the following commands: 
```
sudo apt install cmake
sudo apt install make
```

## How to build
Extract all the files to an empty directory and open bash in that directory.
To build the program, type the following commands:
```
mkdir build
cd build
cmake ../
make
```
The shell can now be started by calling `./shell`

## Concepts

### I/O redirection
*"Redirection can be defined as changing the way from where commands read input to where commands sends output. You can redirect input and output of a command."*
([javatpoint](https://www.javatpoint.com/linux-input-output-redirection))

### System calls
*"In computing, a system call is the programmatic way in which a computer program requests a service from the kernel of the operating system it is executed on. A system call is a way for programs to interact with the operating system."* 
([GeeksforGeeks](https://www.geeksforgeeks.org/introduction-of-system-call/))

- **Execvp:** Execute a shell command, given by an array of tokenized strings. Uses the $PATH environment variable. 
- **Fork:** Create a child process, by duplicating the calling process.
- **Pipe:** Let's you send the output from one command, as the input to another.
- **Dup2:** Alloces a new file descriptor, that duplicates data from the specified file descriptor.
- **Wait:** Wait until child process is finished.
- **Exit:** Exit the child process, or program. Takes an arguement which shows if the progam failed, or was successful (EXIT_SUCCESS, EXIT_FAILURE).
