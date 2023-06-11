#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <string.h>   
#include <unistd.h>    

#define RCVBUFSIZE 8 
#define MAXPENDING 5   
#define MON_SIZE 1000

int monitoring(char *argv[]) {
    int sock;                        
    struct sockaddr_in echoServAddr;
    unsigned short echoServPort;     
    char *servIP;  
    
    servIP = argv[2];        
    echoServPort = atoi(argv[3]);

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&echoServAddr, 0, sizeof(echoServAddr));  
    echoServAddr.sin_family      = AF_INET;    
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);  
    echoServAddr.sin_port        = htons(echoServPort); 

    connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));

    return sock;
}

// Логика работы продавца
void seller(int id, char *argv[]) {
    int m_sock = monitoring(argv), msg_len;
    char msg[MON_SIZE];
    sprintf(msg, "[SELLER %d] Selling stock with id=%d\n", getpid(), id);
    printf("%s", msg);  
    msg_len = strlen(msg); 
    send(m_sock, msg, msg_len, 0);
    close(m_sock);

    sleep(1);
}

void HandleTCPClient(int clntSocket, char *argv[])
{
    char echoBuffer[RCVBUFSIZE];        
    int recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0);

    seller(echoBuffer[0] - '0', argv);

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

        HandleTCPClient(clntSock, argv);
    }
}
