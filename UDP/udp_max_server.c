//
// Created by boli on 25/05/24.
//
//
// Created by boli on 24/05/24.
//
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "time.h"
#include <arpa/inet.h>

const char MESSAGE[] = "Siete nella dimora di kle\n";

int main(int argc, char *argv[]) {
    int simpleSocket = 0;
    int simplePort = 0;
    int returnStatus = 0;
    struct sockaddr_in simpleServer;
    struct sockaddr_in clientName;
    char buffer[256] = "";

    if (2 != argc) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    simpleSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (simpleSocket == -1) {
        fprintf(stderr, "Could not create a socket!\n");
        exit(1);
    } else {
        fprintf(stderr, "Socket created!\n");
    }

    /* retrieve the port number for listening */
    simplePort = atoi(argv[1]);

    /* setup the address structure */
    /* use INADDR_ANY to bind to all local addresses  */
    memset(&simpleServer, '\0', sizeof(simpleServer));
    simpleServer.sin_family = AF_INET;
    simpleServer.sin_addr.s_addr = htonl(INADDR_ANY);
    simpleServer.sin_port = htons(simplePort);

    /*  bind to the address and port with our socket  */
    returnStatus = bind(simpleSocket,(struct sockaddr *)&simpleServer,sizeof(simpleServer));

    if (returnStatus == 0) {
        fprintf(stderr, "Bind completed!\n");
    } else {
        fprintf(stderr, "Could not bind to address!\n");
        close(simpleSocket);
        exit(1);
    }


    socklen_t clientNameLength = sizeof(clientName);
    int maxNum = 0;
    while (1) {
        int num;
        memset(&num, 0, sizeof(num));
        int status = recvfrom(simpleSocket, &num, sizeof(num), 0, (struct sockaddr *) &clientName, &clientNameLength);
        if (status < 0){
            perror("recvfrom fallita");
            continue;
        }

        if (num > maxNum){
            maxNum = num;
        }

        printf("ricevuto: '%d' dal client %s\n", maxNum, inet_ntoa(clientName.sin_addr));
        sendto(simpleSocket, &maxNum, sizeof (maxNum), 0, (struct sockaddr *) &clientName, clientNameLength);
    }
    close(simpleSocket);
}
