all:
	gcc -o sched sched.c queue.c

clean:
	rm -f sched
