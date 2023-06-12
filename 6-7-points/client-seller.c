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
    
    servIP = argv[2];        
    echoServPort = atoi(argv[3]);

    msock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    memset(&echoServAddr, 0, sizeof(echoServAddr));  
    echoServAddr.sin_family      = AF_INET;    
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);  
    echoServAddr.sin_port        = htons(echoServPort); 

    return echoServAddr;
}

// Логика работы продавца
void seller(int id, char *argv[]) {
    char msg[MON_SIZE];
    sprintf(msg, "[SELLER %d] Selling stock with id=%d\n", getpid(), id);
    struct sockaddr_in monAddr = monitoring(argv);
    int msg_len;
    printf("%s", msg); 
    msg_len = strlen(msg); 
    sendto(msock, msg, MON_SIZE, 0, (struct sockaddr *) &monAddr, sizeof(monAddr));

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


    for (;;)
    {
        bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));
        char echoBuffer[RCVBUFSIZE];
        unsigned int clntLen = sizeof(echoClntAddr);        
        int recvMsgSize = recvfrom(servSock, echoBuffer, RCVBUFSIZE, 0, (struct sockaddr *) &echoClntAddr, &clntLen);

        seller(echoBuffer[0] - '0', argv);
        printf("%d", echoBuffer[0] - '0');

    }
    close(servSock);
}
