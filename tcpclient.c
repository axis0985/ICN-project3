#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAX_MSG 52

short f_to_buf(FILE *f, char *buf) {
    char ch;
    memset(buf, 0, sizeof(buf));
    for (int i=0; i<MAX_MSG; i++) {
        ch = fgetc(f);
        buf[i] = ch;
        if (ch == EOF) {
            return 1;
        }
    }
    return 0;
}

int main(int argc , char *argv[])
{
    char serv_addr_str[100];
    printf("Please Input Server Address: ");
    memset(serv_addr_str, 0x0, 100);
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
    memset(&info, 0x0, sizeof(info));
    info.sin_family = PF_INET;

    //localhost test
    info.sin_addr.s_addr = inet_addr(serv_addr_str);
    info.sin_port = htons(1500);


    int err = connect(sockfd,(struct sockaddr *)&info, sizeof(info));
    if(err==-1){
        printf("Connection error");
    }


    //Send a message to server
    FILE *fptr;
    char buf[MAX_MSG];
    memset(buf, 0x0, sizeof(buf));
    fptr = fopen("data", "r");
    short boolean = 0;
    char message[] = {"Hi there"};
    char receiveMessage[100] = {};

    while (boolean == 0) {
        boolean = f_to_buf(fptr, buf);
        if (send(sockfd, buf, sizeof(buf),0) < 0)
        {
            printf("Errrrr");
            break;
        }
    }
    printf("end");
    close(sockfd);
    return 0;
}
