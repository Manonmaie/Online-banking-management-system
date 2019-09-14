server:
	gcc server.c -o server
	./server
client:
	gcc client.c -o client
	./client
start:
	gcc dummy_data.c -o start
	./start
clean:
	rm server client start
	rm JointAccountUser NormalUser Administrator
