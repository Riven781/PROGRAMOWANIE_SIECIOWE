#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //memset()
#include <unistd.h> //read(), write(), close()
#include <arpa/inet.h> //biblioteka do konwersji adresow IP   htonl()  htons()
#include <netinet/in.h> //definiuje struktury i stałe do pracy z IPv4/IPv6.   struct sockaddr_in
#include <sys/socket.h>  // socket() bind()  listen() accept()

//    ./zad5 20123

int main(int argc, char const *argv[]){
    int lst_sock;   // gniazdko nasłuchujące
    int clnt_sock;  // gniazdko połączone z bieżącym klientem
    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // wyniki zwracane przez read() i write() są tego typu

    lst_sock = socket(AF_INET, SOCK_STREAM, 0); //SOCK_STREAM oraz 0 - domyslnie protokol TCP
    if (lst_sock == -1){
        perror("socket");
        return 1;
    }

    //addr to zmienna typu sockaddr_in (a to jest struktura)
    // = {} inicjalizacja przy tworzeniu zmiennej
    struct sockaddr_in addr = {
        .sin_family = AF_INET,  //typ adresu
        .sin_addr = { .s_addr = htonl(INADDR_ANY) }, //tu okreslamy adres IP INADDR_ANY - nasuchuj na wszystkich interfejsach sieciowych  
        .sin_port = htons(atoi(argv[1])) //port
    };

    rc = bind(lst_sock, (struct sockaddr *) & addr, sizeof(addr)); //bind mowi ten socket ma używać tego adresu IP i tego portu
    //rzutowanie, czyli zmiana typu wskaznika z struct sockaddr_in * na struct sockaddr *

    if (rc == -1) {
        perror("bind");
        return 1;
    }

    

    rc = listen(lst_sock, 10);  //10 to maksymalna liczba oczekujacych poloaczen
    if (rc == -1) {
        perror("listen");
        return 1;
    }

    bool keep_on_handling_clients = true;
    while (keep_on_handling_clients) {

        clnt_sock = accept(lst_sock, NULL, NULL); //odebranie połączenia od klienta, który wcześniej zgłosił się do serwera.
        if (clnt_sock == -1) {
            perror("accept");
            return 1;
        }

        const char hello_message[] = "Hello, world!\r\n";
        //const char hello_message[] = "Hello, w\14rld!\r\n";

        cnt = write(clnt_sock, hello_message, sizeof(hello_message) - 1);
        if (cnt == -1) {
            perror("write");
            return 1;
        }
        printf("wrote %zi bytes\n", cnt);

        rc = close(clnt_sock);
        if (rc == -1) {
            perror("close");
            return 1;
        }

    }

    rc = close(lst_sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }

    return 0;

}

/*
Mozna zapisac adres klienata:

struct sockaddr_in client_addr;
socklen_t client_len = sizeof(client_addr);  //moze zostac zmieniona na rzeczywistą liczbę bajtów zapisanych w strukturze client_addr (funkca accept moze to zmienic)

clnt_sock = accept(lst_sock,
                   (struct sockaddr*)&client_addr,
                   &client_len);
*/

/*
printf("Client IP: %s\n", inet_ntoa(client_addr.sin_addr));
printf("Client port: %d\n", ntohs(client_addr.sin_port));
*/


//gdy wysle na zly adres (nr portu) to po jakims czasie dostaje info ze sie nie udalo (ale dlugo trzeba czekac)