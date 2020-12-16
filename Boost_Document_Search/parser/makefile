FLAG= -std=c++11 -lboost_filesystem  -lboost_system

parse:parser.cc
	g++ $^ -o $@ $(FLAG)

.PHONY:clean

clean:
	rm parse
