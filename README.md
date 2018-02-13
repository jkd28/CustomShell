# Custom Shell
This was a project for my Introduction to System Software course at the University of Pittsburgh (COE 0449).  The goal of the assignment was to get practical experience programming with the `fork()` command and the use of system calls by creating a very basic shell.  The shell supports UNIX commands, changing directory, time command, and input/output redirection on the command line.

### Usage
Compile: `gcc -o myshell myshell.c`  
Execute: `./myshell`

Example:
```
$ gcc -o myshell myshell.c
$ ./myshell
  > pwd
  /home/jdott/Documents/CustomShell
  > ls
  myshell  myshell.c
  > ls -a
  .  ..  .git  myshell  myshell.c
  > cd ../
  > pwd
  /home/jdott/Documents
  > exit
```
