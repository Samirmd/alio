SRC = os.cpp

ALL_SRC = $(SRC)
ALL_OBJ = $(ALL_SRC:cpp=o)

CXX      = icpc
CXXFLAGS = -g -I ..
LIBNAME	 = libtools.a
LIBS	 = -L/lib -L/usr/lib64 -Bdynamic -lstdc++


.phoney: client clean

tools:	 $(LIBNAME)

$(LIBNAME): $(ALL_OBJ)
	ar r $(LIBNAME) $(ALL_OBJ)


clean:
	rm -f $(ALL_OBJ) $(LIBNAME)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<