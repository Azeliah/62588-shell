Requirements: Cmake, make

How to install Cmake and make:
-Type "sudo apt install Cmake".
-Type "sudo apt install make".


To use the shell, download the zipfile, and extract it. Open your terminal, and direct yourself to the directory in which you have the files. 
Type "mkdir build", followed by "cd build", and finally "cmake --/". Now you will have a makefile in the directory.
Type "make" and now you should have a file called "Shell". Type "./Shell" and now your shell should be working.



---Concepts---

I/O redirection: 
"Redirection can be defined as changing the way from where commands read input to where commands sends output. You can redirect input and output of a command." (Javapoint.com)


System calls:
"In computing, a system call is the programmatic way in which a computer program requests a service from the kernel of the operating system it is executed on. 
A system call is a way for programs to interact with the operating system." (Geeksforgeeks.com)


-Execvp:
Execute a shell command, given by an array of tokenized strings. Uses the $PATH variable. 

-Fork:
Create a child process, by duplicating the calling process.

-Pipe:
Let's you send the output from one command, as the input to another.

-Dup2:
Alloces a new file descriptor, that duplicates data from the specified file descriptor.

-Wait:
Wait untill child process is finished.

-Exit:
Exit the child process, or program. Takes an arguement which shows if the progam failed, or was successfull(EXIT_SUCCESS, EXIT_FAILURE).