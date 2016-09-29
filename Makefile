CC = gcc
CFLAGS = -O0 -std=gnu99 -Wall

EXECUTABLE = clz

default:
	$(CC) $(CFLAGS) clz.c -o clz

gentxt: default
	./clz \
	> output.txt

plot: gentxt
	# gnuplot runtime.gp
clean:
	rm -f $(EXECUTABLE) *.o *.txt runtime.png
