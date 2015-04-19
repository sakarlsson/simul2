CFLAGS = -g

OBJS = car.o circuit.o lap.o simul2.o a.out 


simul2: $(OBJS)
	$(CC) $(CFLAGS) -o simul2 $(OBJS) -lm

clean: 
	rm -f $(OBJS)

tests:
