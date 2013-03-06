all:
	g++ -o test -I/usr/local/include -I/usr/local/include/cppconn -lmysqlcppconn *.cpp
clean:
	rm -f test
	
