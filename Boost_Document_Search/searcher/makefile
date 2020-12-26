FLAG=-std=c++11 -I ~/chen/Linux/Boost_Document_Search/include  -ljsoncpp

all:index_test searcher_test libsearcher.so 
index_test:index_test.cc searcher.cc
	g++ $^ -o $@ $(FLAG) 
searcher_test:searcher_test.cc searcher.cc
	g++ $^ -o $@ $(FLAG) 
libsearcher.so:searcher.cc searcher.hpp
	g++ searcher.cc -o $@ -shared -fPIC  $(FLAG)

clean:
	rm index_test searcher_test libsearcher.so
