#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */

//for Mac OS X
#include <stdlib.h>
#include "myHeader.h"
#define LOCAL_SERVER_PORT 1500
#define MAX_MSG 100

void print_myHeader(myHeader *hdr)
{
    printf("myHeader field1: %d\n", hdr->field1);
    printf("myHeader field2: %d\n", hdr->field2);
    printf("myHeader field3: %d\n", hdr->field3);
    printf("myHeader field4: %d\n", hdr->field4);
    printf("myHeader field5: %d\n", hdr->field5);
}

int main(int argc, char *argv[])
{
    int fd_socket;
    struct sockaddr_in servAddr;

    /* socket creation, Get a socket into UDP/IP */
    if( (fd_socket=socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        printf("%s: cannot open socket \n",argv[0]);
        exit(1);
    }

    /* bind local server port */
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 利用 htonl, htons 轉成網絡字節優先順序
    servAddr.sin_port = htons(LOCAL_SERVER_PORT); // 設定 port 為 0 的話，電腦會自動搜尋一個未被佔用的 port
    
    /* Bind to the address to which the service will be offered */
    if( bind(fd_socket, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) 
    {
        printf("%s: cannot bind port number %d \n", argv[0], LOCAL_SERVER_PORT);
        exit(1);
    }

    printf("%s: waiting for data from client\n", argv[0]);

    /* server infinite loop */
    char msg[MAX_MSG];
    myHeader	hdr;
    int cliLen, nbytes;
    struct sockaddr_in cliAddr;
    while(1) 
    {

        /* init buffer */
        memset(msg, 0x0, MAX_MSG);
        memset(&hdr, 0x0, sizeof(myHeader));

        /* receive message */
        cliLen = sizeof(cliAddr);
        nbytes = recvfrom(fd_socket, &hdr, sizeof(myHeader), 0, (struct sockaddr *) &cliAddr, &cliLen);
        if(nbytes<0) 
        {
            printf("%s: cannot receive data \n",argv[0]);
            continue;
        }

        print_myHeader(&hdr);
        printf("\n");
    }/* end of server infinite loop */

    return 0;
}




