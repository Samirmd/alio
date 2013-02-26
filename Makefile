CXX		 = icpc
CXXFLAGS = -g -I .
LIBS 	 = client/libclient.so tools/libtools.a
LIBS 	 = 


.phoney: tools_lib server_lib client_lib clean

default: tools_lib client_lib server_lib main.o
	$(CXX) main.o $(LIBS) -o main

tools_lib:
	$(MAKE) -C tools

client_lib:
	$(MAKE) -C client

server_lib:
	$(MAKE) -C server



clean:
	rm -f main.o
	$(MAKE) -C tools  clean
	$(MAKE) -C client clean
	$(MAKE) -C server clean

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
