
#
#  Build the application.
#
kpie: kpie.c Makefile
	gcc -o kpie -Wall kpie.c $(shell pkg-config --cflags --libs  libwnck-1.0) $(shell pkg-config --cflags --libs lua5.1)


#
#  Clean
#
clean:
	rm kpie || true

