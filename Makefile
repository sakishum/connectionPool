all:
	g++ -o test -I/usr/local/include -I/usr/local/include/cppconn -lmysqlcppconn *.cpp

mysql:
	mysql.server start

clean:
	rm -f test
	
