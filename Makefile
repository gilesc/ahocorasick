libahocorasick.so : ahocorasick.cc
	g++ ahocorasick.cpp -fPIC -shared -o libahocorasick.so

install : libahocorasick.so
	cp libahocorasick.so /usr/lib/
	cp ahocorasick.hpp /usr/include/

clean :
	rm *.a *.o *.so
