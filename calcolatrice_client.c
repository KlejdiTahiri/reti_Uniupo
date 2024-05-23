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
#include <arpa/inet.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {

    int simpleSocket = 0;
    int simplePort = 0;
    int returnStatus = 0;
    char buffer[256] = "";
    struct sockaddr_in simpleServer;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server IP> <port>\n", argv[0]);
        exit(1);
    }

    /* create a streaming socket */
    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
    memset(&simpleServer, '\0', sizeof(simpleServer));
    simpleServer.sin_family = AF_INET;
    simpleServer.sin_addr.s_addr = inet_addr(argv[1]);
    simpleServer.sin_port = htons(simplePort);

    /* connect to the address and port with our socket */
    returnStatus = connect(simpleSocket, (struct sockaddr *)&simpleServer, sizeof(simpleServer));
    if (returnStatus == -1) {
        fprintf(stderr, "Could not connect to address!\n");
        close(simpleSocket);
        exit(1);
    }
    else {
        fprintf(stderr, "Connect successful!\n");
    }
    struct timeval starttime, endtime;
    double timeServ;

    int numVolte;
    printf("Metti il numero di connesioni: ");
    scanf("%d", &numVolte);
    gettimeofday(&starttime,NULL);
    write(simpleSocket, &numVolte, sizeof(numVolte));

    while (numVolte >= 0) {
        int num;
        int num1, num2;
        char operazione;
        printf("Scegli l'operazione (+, -, *, /): ");
        scanf(" %c", &operazione);
        printf("Scegli i numeri : ");
        scanf(" %d", &num1);
        printf("Scegli i numeri : ");
        scanf(" %d", &num2);

        switch (operazione) {
            case '+':
            case '-':
            case '*':
            case '/':
                write(simpleSocket, &num1, sizeof(operazione));
                write(simpleSocket, &num2, sizeof(operazione));
                write(simpleSocket, &operazione, sizeof(operazione));
                break;
            default:
                printf("operazione non valida\n");
                exit(1);
        }


        returnStatus = read(simpleSocket, &num, sizeof(num));

        if (returnStatus > 0) {
            if (num < 1){
                printf("Lunghezza stringa mandata dal server: %.2f\n", (float )num);
            } else{
                printf("Lunghezza stringa mandata dal server: %d\n", (int )num);
            }
        } else {
            fprintf(stderr, "Errore nel ricevere il risultato dal server\n");

        }
        numVolte--;
    }
    gettimeofday(&endtime,NULL);

    timeServ=(endtime.tv_sec+(endtime.tv_usec/1000000.0)) - (starttime.tv_sec + (starttime.tv_usec/1000000.0));
    printf("tempo di servizio: %f\n", timeServ);
    return 0;
}


