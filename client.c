#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX 100

void recvFile(int sockfd, char filename[])
{
    char newfile[MAX] = "recv_";
    char buff[MAX]; 	// to store message from client
    strcat(newfile, filename);

    FILE *fp;
    fp=fopen(newfile,"wb"); // stores the file content in recieved.txt in the program directory

    if( fp == NULL ){
        printf("Error IN Opening File ");
        return ;
    }

    //while( read(sockfd,buff,MAX) > 0 ) fprintf(fp,"%s",buff);

    ssize_t bytes_read = 0;

    while((bytes_read = read(sockfd, buff, MAX)) > 0) {

        fwrite(buff, 1, bytes_read, fp);
        if (ferror(fp)) {
            perror("Error while writing to file");
            close(sockfd);
            fclose(fp);
            exit(EXIT_FAILURE);
        }
    }
    printf("%d bytes received\n", bytes_read);
    printf("File received successfully !! \n");
    printf("New File created is received.txt !! \n");

}


int main(int argc, char *argv[]) {
    //need to know
    //ip address of the server (defaults to local host)
    //port of the server (33455 for ipv4 and 33446 for ipv6)
    //mtu of the server (buffer size) (1440 for ipv4 and 1280 ipv6)
    //file name to be transferred from the server (default = fileToTransfer)
    // 0000:0000:0000:0000:0000:0000:0000:0001


    //TODO: Fix MTU things


    char* server_name = "127.0.0.1";  // for local host (loopback)
    int server_port = 0;
    int connection_type = 0;
    char* file_name = "fileToTransfer";
    int mtu = 0;

    if(argc == 1)
    {
        printf("USAGE: "
               "Connection Type + IPv4/IPv6 Address + Port + Buffer Size + Filename\n"
               "Items will default to their default values if they are skipped\n"
               "To skip an item type '.' instead\n"
               "Connection Type is required. Input 4 for IPv4 or 6 for IPv6\n"
               "Input IPv4 addresses as example: 192.168.1.254\n"
               "Input IPv6 addresses as example: AA:BB:CC:DD:EE:FF\n"
               "Default Server Address : 127.0.0.1\n"
               "Example: 4 192.168.0.1 8000 1500 filename.txt\n");
    }

    //for(int i = 0; i < argc; i++){printf("%s\n", argv[i]);}

    //parse command line arguments
    if(!(strcmp(argv[1], "4") == 0 || strcmp(argv[1], "6") == 0))
    {
        printf("Please use a connection of 4 or 6.\n");
    }
    else {
        printf("Connection Type: IPV%i\n", atoi(argv[1]));
    }

    if (strcmp(argv[2], ".") == 0) {
        printf("Server IP: %s (Default)\n", server_name);
    } else {
        server_name = argv[2];
        printf("Server IP: %s\n", server_name);
    }

    if (strcmp(argv[3], ".") == 0) {
        if(strcmp(argv[1], "4") == 0) {
            server_port = 33455;
            printf("Server Port: %i (Default IPV4)\n", server_port);
        }
        else if (strcmp(argv[1], "6") == 0) {
            server_port = 33446;
            printf("Server Port: %i (Default IPV6)\n", server_port);
        }
    }
    else {
        server_port = atoi(argv[3]);
        printf("Server Port: %i\n", server_port);
    }

    if (strcmp(argv[4], ".") == 0) {
        if(strcmp(argv[1], "4") == 0) {
            mtu = 1440;
            printf("MTU: %i (Default IPV4)\n", mtu);
        }
        else if (strcmp(argv[1], "6") == 0) {
            mtu = 1280;
            printf("MTU: %i (Default IPV6)\n", mtu);
        }
    } else {
        mtu = atoi(argv[4]);
        printf("MTU: %i\n", mtu);
    }

    if (strcmp(argv[5], ".") == 0) {
        printf("File: %s (Default)\n", file_name);
    } else {
        file_name = argv[5];
        printf("File: %s\n", file_name);
    }


    //set up for IPv4
    if(strcmp(argv[1], "4") == 0)
    {
        struct sockaddr_in server_address;
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        inet_pton(AF_INET, server_name, &server_address.sin_addr);
        server_address.sin_port = htons(server_port);

        int sockfd;
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("The socket was unable to be created.");
            return 1;
        }
        else{
            printf("The socket was created successfully.\n");
        }


        if (connect(sockfd, (struct sockaddr*)&server_address,
                    sizeof(server_address)) < 0) {
            printf("Could not connect to the specified server.\n");
            return 1;
        }

        const char* data_to_send = file_name;
        send(sockfd, data_to_send, strlen(data_to_send), 0);

        recvFile(sockfd, file_name);


        // close the socket
        close(sockfd);
        return 0;
    }

    //set up for IPv6
    else if(strcmp(argv[1], "6") == 0)
    {

    }
}
