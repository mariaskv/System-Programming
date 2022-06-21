all: compile

compile:
	gcc -o dataServer -pthread ./server.c ../modules/ADTQueue.c
	gcc -o remoteClient ./client.c

executeServer:
	./dataServer -p 9002 -s 10 -q 1 -b 10

executeClient:
	./remoteClient -i 127.0.0.1 -p 9002 -d ../watch

clean:
	rm -f src/*.o
	rm -f ../output/*
	rm -f dataServer
	rm -f remoteClient


