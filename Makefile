CXX=g++
# CXXFLAGS= -std=c++11 -Wall -pedantic -g
CXXFLAGS=-Wall -pedantic -g
LIBS=-lpthread
TARGET = ./bin

all: $(TARGET)/seqtest $(TARGET)/speedtest $(TARGET)/thrtest

$(TARGET)/seqtest: seqmain.cpp solution.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

$(TARGET)/speedtest: speedmain.cpp solution.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

$(TARGET)/thrtest: thrmain.cpp solution.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f core *~ *.o seqtest speedtest thrtest
