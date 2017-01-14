CC = gcc

default: 
	$(CC) -o lab0 -g lab0.c

clean:
	rm -f lab0 test_output.txt

check:
	./makecheckscript

dist:
	tar -cvzf lab0-123456789.tar.gz lab0.c README Makefile \
            segfault-backtrace.png breakpoint-inspect.png test_input.txt \
	    makecheckscript