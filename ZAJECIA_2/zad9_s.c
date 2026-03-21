#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

//    ./zad9s 20123

int main(int argc, char const *argv[])
{
    int rc;
    ssize_t cnt;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1){
        perror("lst socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = htonl(INADDR_ANY) },
        .sin_port = htons(atoi(argv[1]))
    };

    rc = bind(sock, (struct sockaddr *) &addr, sizeof(addr));

    if(rc == -1){
        perror("bind");
        return 1;
    }


    bool keep_on_handling_clients = true;
    while (keep_on_handling_clients)
    {
        unsigned char buf[16];
        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_len = sizeof(clnt_addr);

        cnt = recvfrom(sock, buf, 16, 0, (struct sockaddr *) &clnt_addr, &clnt_addr_len);

        if (cnt == -1){
            perror("recevefrom");
            return -1;
        }

        printf("received %zi bytes\n", cnt);

        memcpy(buf, "Hello, world!\r\n", 15);//15 bajtow bo \0 nie kopiujemy
        //memcpy(buf, "H\14llo, world!\r\n", 15); 

        cnt = sendto(sock, buf, 15, 0, (struct sockaddr *) &clnt_addr, clnt_addr_len);

        if(cnt == -1){
            perror("sendto");
            return -1;
        }

    }

    rc = close(sock);

    if(rc == -1){
        perror("close");
        return 1;
    }


    return 0;
}


//Kolejkują się tylko pakiety, które zmieszczą się w buforze