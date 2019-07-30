CC := g++
OPTIONS := -O2 -I/usr/include/X11/ -L/usr/lib/ -lX11 -lm
VERSION := -std=c++14

enumTsp: tspShow.cpp
	$(CC) $(VERSION) $(OPTIONS) $^ $(OPTIONS) $@.cpp -o $@

clean:
	$(RM) enumTsp

origin: origin/tspShow.c origin/enumTsp.c
	gcc $(OPTIONS) origin/tspShow.c  $(OPTIONS) origin/enumTsp.c -o enumTsp_or

debug: tspShow.cpp
	$(CC) $(VERSION) -g $(OPTIONS) $^ $(OPTIONS) enumTsp.cpp -o enumTsp

