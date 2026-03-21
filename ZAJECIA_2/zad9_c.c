#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/time.h>

//  ./zad9c 0x7F000001 20123

int main(int argc, char const *argv[])
{

    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // na wyniki zwracane przez recvfrom() i sendto()

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock == -1){
        perror("socket");
        return 1;
    }

    struct timeval timeout = {
        .tv_sec = 2
    };

    cnt = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)); //po 2 sekundach recvfrom jesli nie dostanie danych to zwroci -1

    if(cnt < 0){
        perror("setsockopt");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = {.s_addr = htonl(strtoul(argv[1], NULL, 0))},
        .sin_port = htons(atoi(argv[2]))
    };

    unsigned char buf[16];
    //memcpy(buf, "Hello world\r\n", 4);

    //cnt = sendto(sock, buf, 4, 0, (struct sockaddr *) &addr, sizeof(addr));
    cnt = sendto(sock, NULL, 0, 0, (struct sockaddr *) &addr, sizeof(addr)); //wyslanie datagramu z pustym payloadem

    if(cnt == -1){
        perror("sendto");
        return 1;
    }

    cnt = recvfrom(sock, buf, 16, 0, NULL, NULL);

    if(cnt == -1){
        perror("recvfrom");
        return 1;
    }

    unsigned char printable_buf[16];
    int printable_i = 0;
    for (int i = 0; i < cnt; i++)
    {
        if (buf[i] >= 32 && buf[i] <= 126 || buf[i] == '\t' || buf[i] == '\r' || buf[i] == '\n'){
            printable_buf[printable_i] = buf[i];
            ++printable_i;
        }
    }

    printable_buf[printable_i] = '\0';
    ++printable_i;
    printf("%s", printable_buf);

    rc = close(sock);

    if(rc == -1){
        perror("close");
        return 1;
    }

    return 0;
}


//gdy wysle na zly adres to czekam w nieskonczonosc