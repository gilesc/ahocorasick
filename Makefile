libahocorasick.so : ahocorasick.cpp
	g++ -std=c++0x ahocorasick.cpp -fPIC -shared -o libahocorasick.so

ahocorasick :
	g++ -std=c++0x ahocorasick.cpp -o ahocorasick

install : libahocorasick.so
	cp libahocorasick.so /usr/lib/
	cp ahocorasick.hpp /usr/include/

clean :
	rm *.a *.o *.so
