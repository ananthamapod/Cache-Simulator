all: sim

SRCS	= sim.c valid.c simulate.c
OBJS	= ${SRCS:.c=.o}
.c.o:
	gcc -c -ansi -pedantic $<

sim: $(OBJS)
	gcc $(OBJS) -o sim

clean:
	rm -rf $(OBJS)  sim

