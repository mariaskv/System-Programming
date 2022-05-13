Syspro - Assignment 1 - Fall 2021-2022

Skevofylaka Maria - 1115201900173

Part 1:

Compile - Execution:

make compile : Compile all the files and create the executable sniffer

make execute : Execute the sniffer executable

make clean : Remove all the non-source code files

make valgrind : Execute the sniffer executable using valgrind

/src:

main.c : This file includes the manager impelementation

listener.c : This file executes the inotifywait command only for created and moved files. The monitored file is the file ./Inputs 

worker.c : This file recevies a file's path opens it, and search for urls into it, then writes them down in a new file named "filename".out. All the .out files are saved into the folder ./Output
of the initial project. 

The named pipes are named as : pid.fifo where pid is the pid of the child process in which it referes to.

/modules

ADTQueue.c : This file includes the Queue code. This Queue stores the forked children as nodes with them pid and a pointer to the next node.

DISCLAIMER: The implementation of the Queue is based and inspired on the implementation of a Linked List that I wrote for my project 1 of the lesson of Object Oriented Programming in Winter 2020.
This implementation can be found on my Github (here : https://rb.gy/pwtyvb ) and the code has been written 100% by me. 

Part 2:
The shell script named finder.sh takes a parametre from the command line. The first parametre refers to the TLD name (eg. "com"). After that it itterates over every file under the folder ./Output
and for each file prints how many times that given TLD appears.
