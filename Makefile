CXX = g++

MESSAGE = ./message/message.cpp
MAIN = ./main.cpp
ALL = $(MESSAGE) $(MAIN)

#Libraries
LIBS = -lzmq -pthread 
#Flags for g++
CXXFLAGS = -Wall 


main: clean debug 
	$(CXX) $(CXXFLAGS) $(ALL) $(LIBS) 

clean:
	 rm -f *.out

debug:
	echo "Compiling project"
