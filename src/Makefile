
CXX		= g++
CXXFLAGS= -g -Wall
CLS_SRC	= $(wildcard ./*.cpp)
OBJECTS = $(CLS_SRC:.cpp=.o)
EXE    	= RiggedChess

all: $(CLS_SRC) $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

clean:
	rm -f ./${EXE}
	rm -f ./*.o 
