#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <string.h>   
#include <unistd.h>    

#define RCVBUFSIZE 1000  
#define MAXPENDING 5

void send_to_client(char *msg, char * servIP, unsigned short echoServPort, struct sockaddr_in echoClntAddr)
{
    int sock;                        
    struct sockaddr_in echoServAddr;    
    char echoBuffer[RCVBUFSIZE];     
    int bytesRcvd, totalBytesRcvd;

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    memset(&echoServAddr, 0, sizeof(echoServAddr));  
    echoServAddr.sin_family      = AF_INET;    
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);
    echoServAddr.sin_port        = htons(echoServPort); 
 
    sendto(sock, msg, RCVBUFSIZE, 0, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));

    close(sock);
}

void send_to_clients(char *msg, int argc, char *argv[], struct sockaddr_in echoClntAddr)
{
    int sock;                        
    struct sockaddr_in echoServAddr;
    unsigned short echoServPort;     
    char *servIP;                    
    char echoBuffer[RCVBUFSIZE];     
    int bytesRcvd, totalBytesRcvd;   
    
    servIP = argv[2];        
    echoServPort = atoi(argv[3]);
    int i = 2;
    while (i < argc) {
        send_to_client(msg, argv[i], atoi(argv[i+1]), echoClntAddr);
        i++;
        i++;
    }

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    memset(&echoServAddr, 0, sizeof(echoServAddr));  
    echoServAddr.sin_family      = AF_INET;    
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);
    echoServAddr.sin_port        = htons(echoServPort); 
 
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

    printf("[MONITORING %d] Started\n", getpid());

    echoServPort = atoi(argv[1]);

    servSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    memset(&echoServAddr, 0, sizeof(echoServAddr));  
    echoServAddr.sin_family = AF_INET;               
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServAddr.sin_port = htons(echoServPort);    


    for (;;)
    {
        bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));
        clntLen = sizeof(echoClntAddr);

        char echoBuffer[RCVBUFSIZE];      
        int recvMsgSize = recvfrom(servSock, echoBuffer, RCVBUFSIZE, 0, (struct sockaddr *) &echoClntAddr, &clntLen);
    
        while (recvMsgSize > 0)  
        {
            printf("[MONITORING %d] %s", getpid(), echoBuffer);
            send_to_clients(echoBuffer, argc, argv, echoServAddr);
            recvMsgSize = recvfrom(servSock, echoBuffer, recvMsgSize, 0, (struct sockaddr *) &echoClntAddr, &clntLen);
        }

    }
    close(servSock);
}
