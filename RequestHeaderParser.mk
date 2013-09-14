CXX = g++ -std=gnu++03
CXXFLAGS = -Wall -g -D REQUESTHEADERPARSER_UNIT_TEST 
INCLUDES = 
LIBS =
OBJS = RequestHeaderParser.o
PROGRAM = RequestHeaderParser.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
