std=StdTcp.c StdThread.c DoubleLinkList.c ../GlobalMessage.c DataCenter.c StdSqlite.c

client:${std} tcpclient.c
	gcc ${std} tcpclient.c -o client -lStdFile -lMyString -lsqlite3

.PHONY:all clean

all:client

clean:
	rm -f client
