SRC = config.cpp standard_file_object.cpp wrapper.cpp \
      file_object_info.cpp  remote.cpp init.cpp       \
      timer_file_object_decorator.cpp

ALL_SRC = $(SRC)
ALL_OBJ = $(ALL_SRC:cpp=o)

CXX      = mpic++
CXXFLAGS = -g -I .. -fPIC
LIB_SO	 = libclient.so
LIBS	 = ../tools/message.o ../tools/os.o  -ldl -lstdc++


.phoney: client clean

client:	 $(LIB_SO)

$(LIB_SO): $(ALL_OBJ)
	$(CXX) -G $(ALL_OBJ) $(LIBS) -o $(LIB_SO) 


clean:
	rm -f $(ALL_OBJ) $(LIB_SO)

file_object_info.o:	file_object_info.hpp mirror.hpp \
                    timer_file_object_decorator.hpp

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<