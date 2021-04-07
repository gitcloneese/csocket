all:server client

client:client.c
	g++ -g -o client client.c


server:server.c
	g++ -g -o server server.c


