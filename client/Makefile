SRC = config.cpp standard_file_object.cpp wrapper.cpp \
	file_object_info.cpp  remote.cpp

ALL_SRC = $(SRC)
ALL_OBJ = $(ALL_SRC:cpp=o)

CXX      = icpc
CXXFLAGS = -g -I .. -fPIC
LIB_SO	 = libclient.so
LIBS	 = -L/lib -L/usr/lib64 -Bdynamic -lstdc++


.phoney: client clean

client:	 $(LIB_SO)

$(LIB_SO): $(ALL_OBJ)
	$(CC) -G $(ALL_OBJ) -o $(LIB_SO) -ldl -lstdc++ \
		  -L/apps/openmpi/1.4.3/lib -lmpi_cxx -lmpi -lopen-rte -lopen-pal


clean:
	rm -f $(ALL_OBJ) $(LIB_SO)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<