objects = main.o timerservice.o udp.o src-gen/Tictoc.o

.PHONY: clean

all: YET

clean:
	rm -f $(objects) YET

YET: $(objects)
	gcc -g -o YET $(objects)
	
$(objects): %.o : %.c
	gcc -Wall -c -O3 -o $@ $<