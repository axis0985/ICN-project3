// tcpclient.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc , char *argv[])
{

    char serv_addr_str[100];
    printf("Please Input Server Address: ");
    memset(serv_addr_str,0,100);
    //scanf("%s", &serv_addr_str);
    fgets(serv_addr_str, sizeof(serv_addr_str), stdin);

    //socket的建立
    int sockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd == -1){
        printf("Fail to create a socket.");
    }

    //socket的連線

    struct sockaddr_in info;
    bzero(&info,sizeof(info));
    info.sin_family = PF_INET;

    //localhost test
    info.sin_addr.s_addr = inet_addr(serv_addr_str);
    info.sin_port = htons(1500);


    int err = connect(sockfd,(struct sockaddr *)&info,sizeof(info));
    if(err==-1){
        printf("Connection error");
    }


    //Send a message to server
    char message[] = {"Hi there"};
    char receiveMessage[100] = {};
    send(sockfd,message,sizeof(message),0);

    recv(sockfd,receiveMessage,sizeof(receiveMessage),0);
    printf("Get message from server: %s",receiveMessage);
    
    close(sockfd);
    return 0;
}
