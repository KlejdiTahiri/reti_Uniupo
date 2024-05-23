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

    while (1) {

        int num;

        printf("Metti il numero: ");
        scanf("%d", &num);
        if (num == 0) {
            printf("Non si può inserire 0\n");
            close(simpleSocket);
            exit(1);
        }

        write(simpleSocket, &num, sizeof(num));
        returnStatus = read(simpleSocket, &num, sizeof(num));

        if (returnStatus > 0) {
            printf("Numero mandato dal server: %d\n", num);
        } else {
            fprintf(stderr, "Errore nel ricevere il risultato dal server\n");

        }
    }

    return 0;
}
