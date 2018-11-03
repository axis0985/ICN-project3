#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <sys/time.h> /* select() */

//for Mac OS X
#include <stdlib.h>
#include "myHeader.h"
#define REMOTE_SERVER_PORT 1500
#define MAX_MSG 100

void print_myHeader( myHeader  *hdr )
{
    printf("myHeader field1: %d\n", hdr->field1);
    printf("myHeader field2: %d\n", hdr->field2);
    printf("myHeader field3: %d\n", hdr->field3);
    printf("myHeader field4: %d\n", hdr->field4);
    printf("myHeader field5: %d\n", hdr->field5);
    printf("\n"); 
}

int main(int argc, char *argv[]) 
{
    /* check command line args */
    if(argc<2) 
    {
        printf("usage : %s <server> <data1> ... <dataN> \n", argv[0]);
        exit(1);
    }

    /* get server IP address (no check if input is IP address or DNS name */
    struct hostent *h;
    h = gethostbyname(argv[1]);
    if(h==NULL) 
    {
        printf("%s: unknown host '%s' \n", argv[0], argv[1]);
        exit(1);
    }
    printf("%s: sending data to '%s' (IP : %s) \n", argv[0], h->h_name,
           inet_ntoa(*(struct in_addr *)h->h_addr_list[0]));

    struct sockaddr_in remoteServAddr;
    remoteServAddr.sin_family = h->h_addrtype;
    memcpy((char *) &remoteServAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT);

    /* socket creation */
    int fd_socket;
    if( (fd_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
        printf("%s: cannot open socket \n",argv[0]);
        exit(1);
    }

    /* bind any port */
    struct sockaddr_in cliAddr;
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 利用 htonl, htons 轉成網絡字節優先順序
    cliAddr.sin_port = htons(0); // 設定 port 為 0 的話，電腦會自動搜尋一個未被佔用的 port

    /* bind 函數會自動去搜尋主機的 ip 位置 */
    if(bind(fd_socket, (struct sockaddr *) &cliAddr, sizeof(cliAddr)) < 0)
    {
        printf("%s: cannot bind port\n", argv[0]);
        exit(1);
    }

    myHeader      hdr;
    memset( &hdr, 0x0, sizeof(myHeader) );
    hdr.field1 = 0x1;
    hdr.field2 = 0x2;
    hdr.field3 = 0x3;
    hdr.field4 = 0x4;
    hdr.field5 = 0x5;
    print_myHeader(&hdr);

    /* send data */
    int sentError = sendto(fd_socket, &hdr, sizeof(myHeader), 0,
                           (struct sockaddr *) &remoteServAddr,
                           sizeof(remoteServAddr));
    if(sentError<0)
    {
        printf("%s: cannot send data.\n",argv[0]);
        close(fd_socket);
        exit(1);
    }

    return 1;
}



