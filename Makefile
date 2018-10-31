all:
	gcc -o sched sched.c queue.c -pthread

clean:
	rm -f sched
