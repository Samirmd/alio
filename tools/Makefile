SRC = os.cpp message.cpp string_utils.cpp

ALL_SRC = $(SRC)
ALL_OBJ = $(ALL_SRC:cpp=o)

CXX      = mpic++
CXXFLAGS = -g -I .. -fPIC -O0
LIBNAME	 = libtools.so
LIBS	 = -L/lib -L/usr/lib64 -Bdynamic -lstdc++


.phoney: client clean

tools:	 $(LIBNAME)

$(LIBNAME): $(ALL_OBJ)
	 $(CXX) -G $(ALL_OBJ) -o $(LIBNAME) -ldl -lstdc++


clean:
	rm -f $(ALL_OBJ) $(LIBNAME)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<