CXX = g++ -fPIC
LIBFLAGS = -L.
SHAREFLAGS = --shared
COMPFLAGS  = -fPIC

all: main_1.out main_2.out

main_1.out: libShared.so main.o
	$(CXX) $(LIBFLAGS) -lShared main.o

libShared.so: class.o file_1.o file_2.o
	$(CXX) $(SHAREFLAGS) class.o file_1.o file_2.o -o libShared.so

main_2.out: main_2.o libNewShared.so
	$(CXX) $(LIBFLAGS) -lNewShared main_2.o

libNewShared.so: file_4.o libShared.so
	$(CXX) $(SHAREFLAGS) file_4.o libShared.so -o libNewShared.so

%o : %.cc
	$(CXX) $(COMPFLAGS) -o $<

clean:
	rm *.o *.so
