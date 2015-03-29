#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int sockfd = 0;
    int bytesReceived = 0;
    char recvBuff[10];
    char str[10];
    memset(recvBuff, '0', sizeof(recvBuff));
    struct sockaddr_in serv_addr;
    unsigned short port = 5000;
    char *servIP;
    char *filename; 
    
    printf("\n\t   SFTP Client ");
    printf("\n\t================\n");
    if(argc < 4){
        printf("\n Error : Expected 3 input parameters <input file><server IP><port number> \n\n ");
        return 1;
    }
    else{   
    filename    = argv[1];
    port        = atoi(argv[3]);
    servIP      = argv[2];

    printf("\nFile Requested\t : %s \n", filename);
    printf("Server IP\t : %s\n",servIP);
    printf("Port Number\t : %u\n\n",port);
    
    // Create a socket first */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
       printf("\n Error : Could not create socket \n");
       return 1;
    }

    /* Initialize sockaddr_in data structure */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port); // port
    if((serv_addr.sin_addr.s_addr = inet_addr(servIP)) == -1)
    {
        printf("Error : Invalid IP server address \n");
        return 1;
    }

    /* Attempt a connection */
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\n Error : Connection Failed \n");
        return 1;
    }
    
    /* Create file where data will be stored */
    FILE *fp;
    write(sockfd,filename,strlen(filename)+1);
    read(sockfd,str,100);
    if(strcmp(str,"error") == 0){
        printf("Error : File not found at Server\n");
        return 1;
    }
    fp = fopen(filename, "wb"); 
    if(NULL == fp)
    {
        printf("Error : Error writing file. Check disk permissions");
        return 1;
    }

    /* Receive data in chunks of 10 bytes */
    while((bytesReceived = read(sockfd, recvBuff, 10)) > 0)
    {
        printf("Received %d bytes \n",bytesReceived);    
        int nmemb ;
        int size;
        if(bytesReceived == 10){
            // break down 10 bytes received from server into two chunks of 5 bytes
            size = 5;
            nmemb = 2;
        }
        else{
            //in the end of file client might receive less than 10 bytes
            size = bytesReceived;
            nmemb = 1;
        }
        
        fwrite(recvBuff,size,nmemb,fp);
    }

    if(bytesReceived < 0)
    {
        printf("\n Error : File Read Error \n");
    }
    else{
        printf("\n File transfer complete successfully! \n");
    }
    }
    return 0;
}

