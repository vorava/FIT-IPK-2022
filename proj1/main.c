// IPK projekt 1 - main.c
// Autor: Vojtěch Orava (xorava02)
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include "system_info.h"
#include <netinet/in.h>

#define MAX_LEN 255

int main(int argc, char *argv[]){
    if(argc != 2)
        return -5;

    char *end;
    int portInt = (int)strtol(argv[1], &end, 10);
    if(*end != '\0') return -4;

    if(portInt < 1024 || portInt > 65535) return -4;

    uint16_t port = (uint16_t)portInt;

    char hostName[MAX_LEN];
    char cpuName[MAX_LEN];
    char cpuLoad[MAX_LEN];
    char input[MAX_LEN];
    memset(input, 0, MAX_LEN);

    if(!getHostName(hostName, MAX_LEN)) return -2;
    if(!getCpuName(cpuName, MAX_LEN)) return -2;
    if(!getCpuLoad(cpuLoad, MAX_LEN)) return -2;

    printf("-------Server Info-------\n");
    printf("Port: %d\n", port);
    printf("Host name: %s\n", hostName);
    printf("CPU name: %s\n", cpuName);
    printf("CPU load: %s\n", cpuLoad);

    int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int opt = 1;
    if(server == 0) return -3;

    if(setsockopt(server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) return -1;

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(server, (struct sockaddr *)&address, sizeof(address)) < 0) return -3;
    if(listen(server, 5) < 0) return -3;

    int newSocket, readSize;
    char message[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n";
    char error[] = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain;\r\n\r\n";
    char output[strlen(message) + MAX_LEN];
    memset(output, 0, strlen(message) + MAX_LEN);
    int addrLen = sizeof(address);
    int order = 1;
    while (true){
        newSocket = accept(server, (struct sockaddr *)&address, (socklen_t*)&addrLen);
        if (newSocket < 0) return -1;

        readSize = read(newSocket, input, MAX_LEN);
        if(readSize <= 0) return -1;

        if(!strncmp("GET /hostname", input, 13)){
            if(!getHostName(hostName, MAX_LEN)) return -2;
            printf("%d) Client requested host name\n", order);
            strcat(output, message);
            strcat(output, hostName);
            write(newSocket, output, strlen(output));
        }
        else if(!strncmp("GET /cpu-name", input, 13)){
            if(!getCpuName(cpuName, MAX_LEN)) return -2;
            printf("%d) Client requested CPU name\n", order);
            strcat(output, message);
            strcat(output, cpuName);
            write(newSocket, output, strlen(output));
        }
        else if(!strncmp("GET /load", input, 9)){
            if(!getCpuLoad(cpuLoad, MAX_LEN)) return -2;
            printf("%d) Client requested CPU load\n", order);
            strcat(output, message);
            strcat(output, cpuLoad);
            write(newSocket, output, strlen(output));
        }
        else{
            printf("%d) Client request is invalid\n", order);
            write(newSocket, error, strlen(error));
        }
        order++;
        close(newSocket);
        memset(input, 0, MAX_LEN);
        memset(output, 0, strlen(message) + MAX_LEN);
    }
    return 0;
}
