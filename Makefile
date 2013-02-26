CXX		 = icpc
CXXFLAGS = -g -I .
LIBS 	 = client/libclient.so tools/libtools.a


.phoney: tools server client clean

default: tools client server main.o
	$(CXX) main.o $(LIBS) -o main

tools:
	$(MAKE) -C tools libtools.a

client:
	$(MAKE) -C client

server:
	$(MAKE) -C server



clean:
	rm -f main.o
	$(MAKE) -C tools  clean
	$(MAKE) -C client clean
	$(MAKE) -C server clean

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<