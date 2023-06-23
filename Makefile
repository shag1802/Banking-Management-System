data :
	rm datamake
	gcc -w datamake.c -o datamake
	./datamake
server:
	gcc -w server.c operations.c -o server
client:
	gcc -w client.c -o client
full:
	gcc -w datamake.c -o datamake
	./datamake
	gcc -w server.c operations.c -o server
	gcc -w client.c -o client
clean:
	rm datamake client server

