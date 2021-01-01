all:server.out client.out

server.out:server.o filetrans.o
	g++ -g server.o filetrans.o -o server.out

client.out:client.o filetrans.o
	g++ -g client.o filetrans.o -o client.out

server.o:server.cpp
	g++ -g -c server.cpp

client.o:client.cpp
	g++ -g -c client.cpp

filetrans.o:filetrans.cpp
	g++ -g -c filetrans.cpp

clean:
	rm *.o *.out