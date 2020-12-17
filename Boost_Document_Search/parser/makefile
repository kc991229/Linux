FLAG= -std=c++11 -lboost_filesystem  -lboost_system

parse:parse.cc
	g++ $^ -o $@ $(FLAG)

.PHONY:clean

clean:
	rm parse
