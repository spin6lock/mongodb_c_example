all:
	gcc --std=c99 -L../mongo-c-driver/ -lmongoc -I/usr/home/spin6lock/opensource/mongo-c-driver/src main.c -o main
	./main
