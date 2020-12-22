FLAG=-std=c++11 -L/usr/lib64/mysql  -ljsoncpp -lmysqlclient -lpthread 
blog_system:blog_system.cc  db.hpp
	g++ -std=c++11 $^ -o$@ $(FLAG)

clean:
	rm blog_system
