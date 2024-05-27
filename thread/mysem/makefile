all:mysem
CFLAGS+= -Wall -g
LDFLAGS+=-pthread -lstdc++

mysem:main.o mysem.o
	gcc $^ $(CFLAGS) $(LDFLAGS) -o $@
clean:
	rm -rf *.o mysem
