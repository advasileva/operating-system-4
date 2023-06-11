#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <string.h>   
#include <unistd.h>    

#define RCVBUFSIZE 1000  
#define MAXPENDING 5

int main(int argc, char *argv[])
{
    int servSock;                   
    int clntSock;                   
    struct sockaddr_in echoServAddr; 
    struct sockaddr_in echoClntAddr;
    unsigned short echoServPort;   
    unsigned int clntLen;   

    printf("[MONITORING %d] Started\n", getpid());

    echoServPort = atoi(argv[1]);

    servSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    memset(&echoServAddr, 0, sizeof(echoServAddr));  
    echoServAddr.sin_family = AF_INET;               
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServAddr.sin_port = htons(echoServPort);    

    bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));

    for (;;)
    {
        clntLen = sizeof(echoClntAddr);

        // clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen);
        char echoBuffer[RCVBUFSIZE];      
        int recvMsgSize = recvfrom(servSock, echoBuffer, RCVBUFSIZE, 0, (struct sockaddr *) &echoClntAddr, &clntLen);
    
        while (recvMsgSize > 0)  
        {
            printf("[MONITORING %d] %s", getpid(), echoBuffer);
            recvMsgSize = recvfrom(servSock, echoBuffer, recvMsgSize, 0, (struct sockaddr *) &echoClntAddr, &clntLen);
        }
    }
}
