SRC        = main.cpp 
TOOLS_SRC  = tools/os.cpp
CLIENT_SRC = client/config.cpp   \
	         client/standard_file_object.cpp client/wrapper.cpp \
			 client/file_object_info.cpp 

ALL_SRC    = $(SRC) $(TOOLS_SRC) $(CLIENT_SRC)

ALL_OBJ = $(ALL_SRC:cpp=o)

CXX	= icpc
CXXFLAGS = -g -I .

.phoney: all clean

all:$(ALL_OBJ)
	$(CXX) $(ALL_OBJ) -o main

clean:
	rm -f $(ALL_OBJ)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<