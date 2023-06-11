#include <stdio.h>      
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h>    
#include <string.h>    
#include <unistd.h> 
#include <sys/types.h>
#include <fcntl.h>

#define RCVBUFSIZE 8
#define MON_SIZE 1000

// DTO покупателя
typedef struct person {
    int list[30];
    int size;
 } person;

int monitoring(char *argv[]) {
    int sock;                        
    struct sockaddr_in echoServAddr;
    unsigned short echoServPort;     
    char *servIP;  
    
    servIP = argv[4];        
    echoServPort = atoi(argv[5]);

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&echoServAddr, 0, sizeof(echoServAddr));  
    echoServAddr.sin_family      = AF_INET;    
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);  
    echoServAddr.sin_port        = htons(echoServPort); 

    connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));

    return sock;
}

// Логика работы покупателя
void buyer(int *list, int size, int sock, char *argv[]) {
    int str_len;
    char str[RCVBUFSIZE];

    char msg[MON_SIZE];
    for (size_t i = 0; i < size; i++) { // Последовательно покупаем все товары из списка
        if (list[i] % 2 == 1) { // Проверяем, в какой отдел нам надо идти
            sprintf(msg, "[BUYER %d] Buying stock %d from 1\n", getpid(), list[i]);
        } else {
            sprintf(msg, "[BUYER %d] Buying stock %d from 2\n", getpid(), list[i]);
        }
        int m_sock = monitoring(argv), msg_len;
        printf("%s", msg);  
        msg_len = strlen(msg); 
        send(m_sock, msg, msg_len, 0);
        close(m_sock);

        sprintf(str, "%d", list[i]);
        send(sock, str, RCVBUFSIZE, 0);
        sleep(3);
    }
    exit(0);
}

// Рекурсивный форк процессов-покупателей
int fork_buyers(person buyers[], int n, int sock, char *argv[]) { 
    if (n == 0) {
        return 0;
    }
    if (fork() == 0) {
        buyer(buyers[n - 1].list, buyers[n - 1].size, sock, argv);
        return 0;
    }
    return fork_buyers(buyers, n - 1, sock, argv);
}

int main(int argc, char *argv[]) {
    int sock;                        
    struct sockaddr_in echoServAddr;
    unsigned short echoServPort;     
    char *servIP;                    
    char *echoString;                
    char echoBuffer[RCVBUFSIZE];    
    unsigned int echoStringLen;      
    int bytesRcvd, totalBytesRcvd;   
    
    servIP = argv[1];        
    echoServPort = atoi(argv[2]);

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&echoServAddr, 0, sizeof(echoServAddr));  
    echoServAddr.sin_family      = AF_INET;    
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);  
    echoServAddr.sin_port        = htons(echoServPort); 

    connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));
    printf("Init buyers client connected to server\n");

     // Ввод информации о покупателях
    int shm, shm_size = 10, n = 0, k = 0, t = 0;
    int input = open(argv[3], O_RDONLY);
    person buyers[20];

    read(input, &n, 1);
    n -= '0';
    for (size_t i = 0; i < n; i++) {
        read(input, &k, 1);
        read(input, &k, 1);
        k -= '0';
        buyers[i].size = k; 
        for (size_t j = 0; j < k; j++) {
            read(input, &t, 1);
            read(input, &t, 1);
            t -= '0';
            buyers[i].list[j] = t;
        }
    }

    fork_buyers(buyers, n, sock, argv);

    close(sock);
    exit(0);
}