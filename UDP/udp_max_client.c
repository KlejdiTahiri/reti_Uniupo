//
// Created by boli on 25/05/24.
//
//
// Created by boli on 24/05/24.
//
#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    int simpleSocket = 0;
    int simplePort = 0;
    int returnStatus = 0;
    char buffer[256] = "";
    struct sockaddr_in	destAddr;
    socklen_t destSize;



    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server IP> <port>\n", argv[0]);
        exit(1);
    }

    /* create a streaming socket */
    simpleSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (simpleSocket == -1) {
        fprintf(stderr, "Could not create a socket!\n");
        exit(1);
    }
    else {
        fprintf(stderr, "Socket created!\n");
    }

    /* retrieve the port number for connecting */
    simplePort = atoi(argv[2]);

    /* set up the address structure */
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_addr.s_addr = inet_addr(argv[1]);
    destAddr.sin_port = htons(simplePort);

    destSize = sizeof(destAddr);
    int sockfd;
    while (1) {

        printf("Inserisci num (-1 per uscire): ");
        scanf("%d", &sockfd);
        if (sockfd == -1) {
            break;
        }
        sendto(simpleSocket, &sockfd, sizeof (sockfd), 0, (struct sockaddr *) &destAddr, destSize);

        int status = recvfrom(simpleSocket, &sockfd, sizeof(sockfd), 0, (struct sockaddr *) &destAddr, &destSize);
        printf("numero max mandato dal server: %d\n", sockfd);

        if (status < 0){
            perror("recvfrom fallita");
            break;
        }

    }
    close(simpleSocket);
    return 0;

}