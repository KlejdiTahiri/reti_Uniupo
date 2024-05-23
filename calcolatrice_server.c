//
// Created by boli on 20/05/24.
//
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
        float num1, num2;
        char operazione;
        returnStatus = read(simpleChildSocket, &NumRicevuto, sizeof(NumRicevuto));

        while (NumRicevuto >= 0) {
            int returnStatus1 = read(simpleChildSocket, &num1, sizeof(operazione));
            int returnStatus2 = read(simpleChildSocket, &num2, sizeof(operazione));
            returnStatus = read(simpleChildSocket, &operazione, sizeof(operazione));
            if (returnStatus <= 0 || returnStatus1 <= 0 || returnStatus2 <= 0) {
                fprintf(stderr, "Errore lettura client!\n");
                close(simpleChildSocket);
                exit(1);
            }

            float res = 0;
            switch (operazione) {
                case '+':
                 res = num1 + num2;
                    break;
                case '*':
                    res = num1 * num2;
                    break;
                case '/':
                    if (num1 < num2){
                        res = (float )num1 / num2;
                    } else{
                        res = num1 / num2;
                    }
                    break;
                case '-':
                    res = num1 - num2;
                    break;
            }



            returnStatus = write(simpleChildSocket,&res, sizeof(res));
            if (returnStatus <= 0) {
                fprintf(stderr, "Error writing highest number to client!\n");
                close(simpleChildSocket);
                exit(1);
            }
            NumRicevuto--;
        }
        close(simpleSocket);
        exit(1);

    }

    return 0;
}

