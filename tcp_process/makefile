all:cli svr 

cli:cli.cpp
	g++ $^ -o $@ -lpthread

svr:svr.cpp
	g++ $^ -o $@ -lpthread

clean:
	rm svr cli

