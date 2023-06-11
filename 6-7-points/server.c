#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <string.h>   
#include <unistd.h>    

#define RCVBUFSIZE 8 
#define MAXPENDING 5
#define MON_SIZE 100

struct sockaddr* monitoring(char *argv[]) {
    int sock;                        
    struct sockaddr_in echoServAddr;
    unsigned short echoServPort;     
    char *servIP;  
    
    servIP = argv[5];        
    echoServPort = atoi(argv[6]);

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_UDP);

    memset(&echoServAddr, 0, sizeof(echoServAddr));  
    echoServAddr.sin_family      = AF_INET;    
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);  
    echoServAddr.sin_port        = htons(echoServPort); 

    connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));

    return &echoServAddr;
}

void send_to_seller(char *msg, char *argv[], struct sockaddr_in echoClntAddr)
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

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_UDP);

    memset(&echoServAddr, 0, sizeof(echoServAddr));  
    echoServAddr.sin_family      = AF_INET;    
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);
    if (atoi(msg) % 2 == 1)
    {
        echoServAddr.sin_port        = htons(echoServPort1); 
    } else {
        echoServAddr.sin_port        = htons(echoServPort2); 
    }
 
    sendto(sock, msg, RCVBUFSIZE, 0, (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr));

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

        char msg[MON_SIZE]; 
        sprintf(msg, "[SERVER %d] Handling client %s\n", getpid(), inet_ntoa(echoClntAddr.sin_addr));
        printf("%s", msg);   
 
        while (recvMsgSize > 0)  
        { 
            int m_sock = monitoring(argv), msg_len;
            char *msg;
            sprintf(msg, "[SERVER %d] Got product with id=%s\n", getpid(), echoBuffer);
            printf("%d", m_sock); 
            msg_len = strlen(msg); 
            sendto(m_sock, msg, MON_SIZE, 0); // TODO 
            close(m_sock);

            sendto(servSock, echoBuffer, recvMsgSize, 0, (struct sockaddr *) &echoClntAddr, clntLen);
            recvMsgSize = recvfrom(servSock, echoBuffer, RCVBUFSIZE, 0, (struct sockaddr *) &echoClntAddr, &clntLen);
            send_to_seller(echoBuffer, argv, echoClntAddr);
        }
    }
}
