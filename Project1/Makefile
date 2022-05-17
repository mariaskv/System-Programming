all: compile

compile:
	rm -f res.txt
	gcc -o sniffer src/main.c modules/ADTQueue.c src/worker.c
	gcc -o listener src/listener.c
	#gcc -o worker src/worker.c

execute:
	./sniffer
valgrind:
	valgrind ./sniffer

clean:
	rm -f src/*.o
	rm -f Inputs/*.txt
	rm -f sniffer
	rm -f res.txt
	rm -f listener
	rm -f worker
	rm -f ./Inputs/Ts.txt
	rm -f ./Output/*.out
	rm -f *.fifo

