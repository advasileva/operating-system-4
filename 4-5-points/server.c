#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <string.h>   
#include <unistd.h>    

#define RCVBUFSIZE 8  
#define MAXPENDING 5

void send_to_seller(char *msg, char *argv[])
{
    int sock;                        
    struct sockaddr_in echoServAddr;
    unsigned short echoServPort1, echoServPort2;     
    char *servIP;                    
    char echoBuffer[RCVBUFSIZE];     
    int bytesRcvd, totalBytesRcvd;   
    
    servIP = argv[2];        
    echoServPort1 = atoi(argv[3]);
    echoServPort2 = atoi(argv[4]);

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&echoServAddr, 0, sizeof(echoServAddr));  
    echoServAddr.sin_family      = AF_INET;    
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);
    if (atoi(msg) % 2 == 1)
    {
        echoServAddr.sin_port        = htons(echoServPort1); 
    } else {
        echoServAddr.sin_port        = htons(echoServPort2); 
    }
    

    connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));
 
    send(sock, msg, RCVBUFSIZE, 0);

    close(sock);
}

void HandleTCPClient(int clntSocket, char *argv[])
{ 
    char echoBuffer[RCVBUFSIZE];      
    int recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0);
 
    while (recvMsgSize > 0)  
    { 
        printf("[SERVER %d] Got product with id=%s\n", getpid(), echoBuffer);  
        send(clntSocket, echoBuffer, recvMsgSize, 0);
        recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0);
        send_to_seller(echoBuffer, argv);
    }

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

        printf("[SERVER %d] Handling client %s\n", getpid(), inet_ntoa(echoClntAddr.sin_addr));

        HandleTCPClient(clntSock, argv);
    }
}
