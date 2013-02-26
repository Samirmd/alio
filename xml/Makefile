SRC = irrXML.cpp  xml_node.cpp

ALL_SRC = $(SRC)
ALL_OBJ = $(ALL_SRC:cpp=o)

CXX      = mpic++
CXXFLAGS = -g -I .. -fPIC -O0
LIB	 	 = libxml.a

.phoney: xml clean

xml:	 $(LIB)

$(LIB): $(ALL_OBJ)
	ar r $(LIB) $(ALL_OBJ)


clean:
	rm -f $(ALL_OBJ) $(LIB)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<