CXX=g++
# CXXFLAGS= -std=c++11 -Wall -pedantic -g
CXXFLAGS=-Wall -pedantic -g 
LIBS=-lpthread
TARGET = ./bin
SOURCE = ./src

all: $(TARGET)/seqtest $(TARGET)/speedtest $(TARGET)/thrtest

$(TARGET)/seqtest: $(SOURCE)/seqmain.cpp $(SOURCE)/solution.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

$(TARGET)/speedtest: $(SOURCE)/speedmain.cpp $(SOURCE)/solution.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

$(TARGET)/thrtest: $(SOURCE)/thrmain.cpp $(SOURCE)/solution.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f core *~ *.o seqtest speedtest thrtest
