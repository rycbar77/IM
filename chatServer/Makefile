all:
	g++ -o chatserver main.cpp dbmysql.h dbmysql.cpp msg.h server.cpp server.h user.cpp user.h thread.cpp thread.h userlist.cpp userlist.h  -lmysqlclient -lpthread
 
clean:
	rm -f *.o chatserver
