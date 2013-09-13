CXX = g++ -std=gnu++03
CXXFLAGS = -Wall -g -D SERVER_UNIT_TEST
INCLUDES = 
LIBS =
OBJS = Server.o
PROGRAM = Server.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
