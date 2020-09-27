CXX = g++

MESSAGE = ./message/message.cpp
TOKEN = ./sk/token.cpp
SK = ./sk/sk.cpp
MAIN = ./main.cpp
ALL = $(MESSAGE) $(TOKEN) $(SK) $(MAIN)

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
