.PHONY:all clean

all:Client Server
#make -C 在指定文件夹下执行对应命令
	make all -C Server
	make all -C Client
	mv ./Server/server ./
	mv ./Client/client ./


clean:
	make clean -C Server
	make clean -C Client
	rm -f server client
