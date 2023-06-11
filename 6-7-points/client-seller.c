#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <string.h>   
#include <unistd.h>    

#define RCVBUFSIZE 8
#define MAXPENDING 5   
#define MON_SIZE 1000
int msock;  

struct sockaddr_in monitoring(char *argv[]) {     
    struct sockaddr_in echoServAddr;
    unsigned short echoServPort;     
    char *servIP;  
    
    servIP = argv[5];        
    echoServPort = atoi(argv[6]);

    msock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    memset(&echoServAddr, 0, sizeof(echoServAddr));  
    echoServAddr.sin_family      = AF_INET;    
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);  
    echoServAddr.sin_port        = htons(echoServPort); 

    // connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));

    return echoServAddr;
}

// Логика работы продавца
void seller(int id, char *argv[]) {
    // int m_sock = monitoring(argv), msg_len;
    char msg[MON_SIZE];
    sprintf(msg, "[SELLER %d] Selling stock with id=%d\n", getpid(), id);
    printf("%s", msg);  
    // msg_len = strlen(msg); 
    // send(m_sock, msg, msg_len, 0);
    // close(m_sock);

    sleep(1);
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

    servSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    memset(&echoServAddr, 0, sizeof(echoServAddr));  
    echoServAddr.sin_family = AF_INET;               
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServAddr.sin_port = htons(echoServPort);    

    bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));

    for (;;)
    {
        char echoBuffer[RCVBUFSIZE];
        unsigned int clntLen = sizeof(echoClntAddr);        
        int recvMsgSize = recvfrom(servSock, echoBuffer, RCVBUFSIZE, 0, (struct sockaddr *) &echoClntAddr, &clntLen);

        seller(echoBuffer[0] - '0', argv);

        close(servSock);
    }
}
