FLAG=-std=c++11 -I ~/chen/Linux/Boost_Document_Search/include  -ljsoncpp

all:index_test searcher_test 
index_test:index_test.cc searcher.cc
	g++ $^ -o $@ $(FLAG) 
searcher_test:searcher_test.cc searcher.cc
	g++ $^ -o $@ $(FLAG) 
clean:
	rm index_test searcher_test
