#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

//   ./zad7 0x7F000001 20123

int main(int argc, char const *argv[])
{
    int sock;
    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // wyniki zwracane przez read() i write() są tego typu

    sock = socket(AF_INET, SOCK_STREAM, 0); //TCP polaczenie
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = htonl(strtoul(argv[1], NULL, 0)) },   // 127.0.0.1 to 0x7F000001   -- adres IP serwera
        .sin_port = htons(atoi(argv[2]))  //port serwera, z którym chcemy się połączyć
    };

    //connect() mowi chcę się połączyć z tym serwerem, na tym IP i porcie
    rc = connect(sock, (struct sockaddr *) & addr, sizeof(addr));

    if (rc == -1) {
        perror("connect");
        return 1;
    }

    unsigned char buf[16];
    
    
    cnt = read(sock, buf, 16);
    if (cnt == -1) {
        perror("read");
        return 1;
    }

    unsigned char printable_buf[17];
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
    printf("read %zi bytes\n", cnt);
    printf("read %i bytes\n", printable_i);
    printf("%s", printable_buf);


    rc = close(sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }

    return 0;
}


/*
Po connect():

- kernel TCP nawiązuje trójstopniowy handshake (SYN, SYN-ACK, ACK) z serwerem

- socket klienta staje się połączony

- możesz używać read() i write() do wymiany danych

*/

//w TCP nie musisz najpierw wysyłać danych do serwera, żeby on mógł Ci cokolwiek wysłać.