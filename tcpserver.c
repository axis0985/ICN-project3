#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MAX_MSG 1280

int main(int argc , char *argv[])

{
    //socket的建立
    int sockfd = 0,forClientSockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        printf("Fail to create a socket.");
    }

    //socket的連線
    struct sockaddr_in serverInfo,clientInfo;
    int addrlen = sizeof(clientInfo);
    memset(&serverInfo, 0x0, sizeof(serverInfo));

    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(1500);
    bind(sockfd, (struct sockaddr *)&serverInfo, sizeof(serverInfo));
    listen(sockfd,5);

    char message[] = {"Hi,this is server.\n"};
    char inputBuffer[MAX_MSG] = {};
    while(1){
        forClientSockfd = accept(sockfd, (struct sockaddr*) &clientInfo, &addrlen);
        recv(forClientSockfd,inputBuffer, sizeof(inputBuffer),0);
        send(forClientSockfd,message, sizeof(message),0);
        printf("Get message from client: %s\n", inputBuffer);
    }
    return 0;
}
