#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <string.h>   
#include <unistd.h>    

#define RCVBUFSIZE 8 
#define MAXPENDING 5   

// Логика работы продавца
void seller(int id) {
    printf("[SELLER %d] Selling stock with id=%d\n", getpid(), id);
    sleep(1);

}

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];        
    int recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0);

    seller(echoBuffer[0] - '0');

    close(clntSocket);
}

int main(int argc, char *argv[])
{
    int servSock;                   
    int clntSock;                   
    struct sockaddr_in echoServAddr; 
    struct sockaddr_in echoClntAddr;
    unsigned short echoServPort;   
    unsigned int clntLen;   

    echoServPort = atoi(argv[1]);

    servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&echoServAddr, 0, sizeof(echoServAddr));  
    echoServAddr.sin_family = AF_INET;               
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServAddr.sin_port = htons(echoServPort);    

    bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));
    listen(servSock, MAXPENDING);

    for (;;)
    {
        clntLen = sizeof(echoClntAddr);

        clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen);

        HandleTCPClient(clntSock);
    }
}
