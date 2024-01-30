std=StdTcp.c StdThread.c DoubleLinkList.c LinkQueue.c StdThreadPool.c ../GlobalMessage.c StdSqlite.c DataCenter.c
server:${std} tcpserver.c
	gcc ${std} tcpserver.c -o server -lsqlite3 -lMyString 


.PHONY:all clean

all:server

clean:
	rm -f server
