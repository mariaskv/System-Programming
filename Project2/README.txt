Syspro - Assignment 2 - Fall 2021-2022

Skevofylaka Maria - 1115201900173

Compile - Execution:

make compile : Compile all the files and create the executables ./dataServer and ./remoteClient

make executeServer : Executes the ./dataServer executable with some given parameters

make executeClient : Executes the ./remoteClient with some given parameters

ALTERNATIVE EXECUTION:

./dataServer -p _ -s _ -q _ -b _ 

Underscores(_) must be replaced with the correct parameters

Also:

./remoteClient -i _ -p _ -d _

IMPORTANT NOTE: The programm works only for parameters given in the correct order as been shown up.

make clean : Remove all the non-source code files

Results:

All the copied files are moved into the folder ./output. There is also given a test file ./watch that includes some files for testing

/src:

server.c: This file includes the code of the server with all the sockets and threads between the server and the clients.

client.c: Includes the client's code that recieves the filenames and do the copy work.

/modules

ADTQueue.c : This file includes the Queue code. This Queue stores the files with theri name and a pointer to the next node.

DISCLAIMER: The implementation of the Queue is based and inspired on the implementation of a Linked List that I wrote for my project 1 of the lesson of Object Oriented Programming in Winter 2020.
This implementation can be found on my Github (here : https://rb.gy/pwtyvb ) and the code has been written 100% by me. 

DISCLAIMER 2: For the implementation of the sockets has been used code from the lecture's pdf.