CC := g++
OPTIONS := -I/usr/include/X11/ -L/usr/lib/ -lX11 -lm

enumTsp: tspShow.cpp
	$(CC) -O2 -std=c++11 $(OPTIONS) $^ $(OPTIONS) $@.cpp -o $@

clean:
	$(RM) enumTsp

origin: origin/tspShow.c origin/enumTsp.c
	gcc -O2 $(OPTIONS) origin/tspShow.c  $(OPTIONS) origin/enumTsp.c -o enumTsp_or

debug: tspShow.cpp
	$(CC) -O2 -std=c++11 -g $(OPTIONS) $^ $(OPTIONS) enumTsp.cpp -o enumTsp
