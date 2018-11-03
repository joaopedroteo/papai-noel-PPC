APPLICATION = main.app
CXXFLAGS = -Wall -I. -std=c++11 -fopenmp

OBJECTS = main.o

# first, try to compile from this project
%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

$(APPLICATION): $(OBJECTS) main.o
	$(CXX) $(CXXFLAGS) $^  -o $@

#gdb:
#	CXXFLAGS = $(CXXFLAGS) -ggdb
#	$(APPLICATION)

clean:
	rm -f *.o *~ $(APPLICATION)
