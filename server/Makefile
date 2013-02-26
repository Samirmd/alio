SRC = server.cpp

ALL_SRC = $(SRC)

ALL_OBJ = $(ALL_SRC:cpp=o)

CXX      = mpic++
CXXFLAGS = -g -I ..

.phoney:  server clean

server: $(ALL_OBJ)
	$(CXX) $(ALL_OBJ) -o server -L ../tools -ltools

clean:
	rm -f $(ALL_OBJ) server

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<