


kpie: kpie.c Makefile
	gcc -o kpie kpie.c $(shell pkg-config --cflags --libs  libwnck-1.0) $(shell pkg-config --cflags --libs lua5.1)

clean:
	rm kpie || true

