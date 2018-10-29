# Linux Makefile for VART Example "Arrow"
# (C) Bruno de Oliveira Schneider - May 2014

# This Makefile requires a local directory "vart" with
# source code for the V-ART framework. Create a symbolic
# link to the real directory and you'll be OK.

APPLICATION = main.app
CXXFLAGS = -Wall -I. -std=c++11

OBJECTS = main.o

# first, try to compile from this project
%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<


$(APPLICATION): $(OBJECTS) main.o
	$(CXX) $^ -o $@

#gdb:
#	CXXFLAGS = $(CXXFLAGS) -ggdb
#	$(APPLICATION)

clean:
	rm -f *.o *~ $(APPLICATION)
