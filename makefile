qqserverd:main.o server.o mthread.o decrypt.o use_mysql.o view.o
	g++ main.o server.o mthread.o decrypt.o use_mysql.o view.o -o qqserverd -ljsoncpp -lmysqlclient -levent  -lpthread -g -lcrypto
main.o:main.cpp
	g++ main.cpp -c 
server.o:server.cpp
	g++ server.cpp -c
mthread.o:mthread.cpp
	g++ mthread.cpp -c
decrypt.o:decrypt.cpp
	g++ decrypt.cpp -c
use_mysql.o:use_mysql.cpp
	g++ use_mysql.cpp -c
view.o:view.cpp
	g++ view.cpp -c
clean:
	rm *.o -rf
