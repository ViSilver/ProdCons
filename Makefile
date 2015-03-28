CXX=g++
# CXXFLAGS= -std=c++11 -Wall -pedantic -g
CXXFLAGS=-Wall -pedantic -g 
LIBS=-lpthread

all: seqtest speedtest thrtest

seqtest: seqmain.cpp solution.cpp
	$(CXX) $(CXXFLAGS) -o bin/$@ $< $(LIBS)

speedtest: speedmain.cpp solution.cpp
	$(CXX) $(CXXFLAGS) -o bin/$@ $< $(LIBS)

thrtest: thrmain.cpp solution.cpp
	$(CXX) $(CXXFLAGS) -o bin/$@ $< $(LIBS)

clean:
	rm -f core *~ *.o seqtest speedtest thrtest
