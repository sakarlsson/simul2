CFLAGS = -g

OBJS = car.o circuit.o lap.o simul2.o

simul2: $(OBJS)
	$(CC) $(CFLAGS) -o simul2 $(OBJS) -lm

clean: 
	rm -f $(OBJS) simul2

test:
	runtest.sh
