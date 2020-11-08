#include <stdlib.h> // for basic memmory allocation and deallocation
#include <stdio.h> // for file read and write
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 100
#define PORT 33455
#define SA struct sockaddr

void sentFile(int sockfd, char filename[])
{
    printf("File Open: %s\n", filename);
    char buff[MAX]; 						// for read operation from file and used to sent operation
    FILE *fp;
    fp=fopen(filename,"rb");
    if( fp == NULL ){
        printf("File Open: Failed\n");
    }

    printf("File Open: Success\n");

    while ( fread(buff,1,MAX,fp) != NULL ) write(sockfd,buff,sizeof(buff));
    fclose (fp);
}


int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        printf("USAGE: "
               "IPV4 Address + Port + IPv6 Address + Port\n"
               "Use '.' to skip an item and use defaults\n");
    }

    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        printf("Socket Creation: Failed\n");
        exit(0);
    }
    else
        printf("Socket Creation: Success\n");

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;					// specifies address family with IPv4 Protocol
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    servaddr.sin_port = htons(PORT); 				// binds to PORT specified

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) != 0) {
        printf("Socket Bind: Failed\n");
        exit(0);
    }
    else
        printf("Socket Bind: Success\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Server Listen: Failed\n");
        exit(0);
    }
    else
        printf("Server Listen: Success\n");

    len = sizeof(cli);

    connfd = accept(sockfd, (struct sockaddr* )&cli, &len);
    if (connfd < 0) {
        printf("Client Connection: Failed\n");
        exit(0);
    }
    else
        printf("Client Connection: Success\n");

    char received[MAX];
    read(connfd, received, MAX);

    printf("File Request: %s\n", received);
    sentFile(connfd, received);
    printf("File Sent: %s\n", received);
    close(sockfd);
}