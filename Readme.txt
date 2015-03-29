Readme 


Server :

1. Compile the server using command:
gcc -o server server.c

2. To start the server give port number :
./server <port number>



Client:

1. Compile client using command:
gcc -o sftp sftp.c

2. Request a file from the server:
./sftp <file name> <IP address of server> <port number of server>
* use localhost 127.0.0.1 as IP address
