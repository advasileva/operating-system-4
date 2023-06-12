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

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    memset(&echoServAddr, 0, sizeof(echoServAddr));  
    echoServAddr.sin_family      = AF_INET;    
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);
    if (atoi(msg) % 2 == 1)
    {
        echoServAddr.sin_port        = htons(echoServPort1); 
    } else {
        echoServAddr.sin_port        = htons(echoServPort2); 
    }

    sendto(sock, msg, RCVBUFSIZE, 0, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));

    close(sock);
}

int main(int argc, char *argv[])
{
    int servSock;                   
    int clntSock;                   
    struct sockaddr_in echoServAddr; 
    struct sockaddr_in echoClntAddr;
    unsigned short echoServPort;   
    unsigned int clntLen;   
    int recvMsgSize;

    echoServPort = atoi(argv[1]);

    servSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    memset(&echoServAddr, 0, sizeof(echoServAddr));  
    echoServAddr.sin_family = AF_INET;               
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServAddr.sin_port = htons(echoServPort);

   bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));

    for (;;)
    { 
    unsigned int clntLen = sizeof(echoClntAddr);

    char echoBuffer[RCVBUFSIZE];
    recvMsgSize = recvfrom(servSock, echoBuffer, RCVBUFSIZE, 0, (struct sockaddr *) &echoClntAddr, &clntLen);  

        while (recvMsgSize > 0)  
        { 
            int msg_len;
            char *msg;
            sprintf(msg, "[SERVER %d] Got product with id=%s\n", getpid(), echoBuffer);

            sendto(servSock, echoBuffer, recvMsgSize, 0, (struct sockaddr *) &echoClntAddr, clntLen);
            recvMsgSize = recvfrom(servSock, echoBuffer, RCVBUFSIZE, 0, (struct sockaddr *) &echoClntAddr, &clntLen);
            send_to_seller(echoBuffer, argv);
        }
    }
}
