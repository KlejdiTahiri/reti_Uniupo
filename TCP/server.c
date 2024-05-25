#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "time.h"

const char MESSAGE[] = "Siete nella dimora di kle\n";

int main(int argc, char *argv[]) {
    int simpleSocket = 0;
    int simplePort = 0;
    int returnStatus = 0;
    struct sockaddr_in simpleServer;
    int maxNum = 0;

    if (2 != argc) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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

    /* lets listen on the socket for connections      */
    returnStatus = listen(simpleSocket, 5);

    if (returnStatus == -1) {
        fprintf(stderr, "Cannot listen on socket!\n");
        close(simpleSocket);
        exit(1);
    }

    while (1) {
        struct sockaddr_in clientName = { 0 };
        int simpleChildSocket = 0;
        int clientNameLength = sizeof(clientName);

        /* wait here */
        simpleChildSocket = accept(simpleSocket,(struct sockaddr *)&clientName, &clientNameLength);

        if (simpleChildSocket == -1) {
            fprintf(stderr, "Cannot accept connections!\n");
            close(simpleSocket);
            exit(1);
        }

        int NumRicevuto;

        while (1) {
            returnStatus = read(simpleChildSocket, &NumRicevuto, sizeof(NumRicevuto));

            if (returnStatus <= 0) {
                fprintf(stderr, "Errore lettura numero client!\n");
                close(simpleChildSocket);
                break;
                //continue;
                //exit(1);
            }

            if (NumRicevuto > maxNum) {
                maxNum = NumRicevuto;
            }

            returnStatus = write(simpleChildSocket, &maxNum, sizeof(maxNum));
            if (returnStatus <= 0) {
                printf(stderr, "Error writing highest number to client!\n");
                close(simpleChildSocket);
                break;
                //exit(1);
            }
        }

    }

    return 0;
}
