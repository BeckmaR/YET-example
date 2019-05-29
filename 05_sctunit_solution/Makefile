objects = main.o timerservice.o trace.o src-gen/Solution05.o

.PHONY: clean

all: Solution05

clean:
	rm -f $(objects) YET

Solution05: $(objects)
	gcc -g -o Solution05 $(objects)
	
$(objects): %.o : %.c
	gcc -Wall -c -O3 -o $@ $<