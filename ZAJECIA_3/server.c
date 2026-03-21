#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

/*
Zalozenia przyjete:
- jesli do serwera zostanie przeslane zapytanie zakonczone \n lub \r\n, serwer rowniez zwroci odpowiedz
    zakonczona \n lub \r\n
- klient nie przesyla zapytan zakonczonych '\0'
- dla kazdego datagramu zawierajacego wiecej niz 1024 bajty zostanie zwrocony ERROR
*/


/*
Stany:
0 - stan poczatkowy
1 - stan tworzenia slowa (przyjecie litery)
2 - stan przyjecia spacji
3 - stan inicjacji terminacji (przyjecie \r)
4 - ERROR
5 - stan akceptacji

Przyjmowane symbole:
0 - litera (np. 'a')
1 - spacja
2 - \n (tylko jako ostatnie)
3 - \r (tylko jako przedostatnie)
4 - \0
5 - niedozwolone wejscie (np. '*')
*/

//funkcja przejscia 
int get_state(unsigned char* symbol, int old_state, int states [][6], int i, int len){
    if ((*symbol >= 65 && *symbol <= 90) || (*symbol >= 97 && *symbol <= 122)){
        return states[old_state][0];
    }
    else if (*symbol == ' '){
        return states[old_state][1];
    }
    else if (*symbol == '\n'){ // \n moze byc tylko ostatnie
        if (i == len - 1){
            return states[old_state][2];
        }
        else{
            return 4; 
        }
    }
    else if (*symbol == '\r'){ // \r moze byc tylko przed ostatnie
        if (i == len - 2){
            return states[old_state][3];
        }
        else{
            return 4; 
        }
    }
    else if (*symbol == '\0'){
        if (i == len){ // \0 moze byc tylko ostatnie (tzn. ze nie ma ani \n ani \r\n)
            return states[old_state][4];
        }
        else{
            return 4;
        }
    }
    else {
        return states[old_state][5];
    }

}

bool is_palidrome(unsigned char* word, int len){
    for (int i = 0; i < len / 2; ++i){
        if (word[i] != word[len - 1 - i]){
            return false;
        }
    }
    return true;
}

int main(int argc, char const *argv[])
{
    int rc;
    ssize_t cnt;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1){
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = htonl(INADDR_ANY) },
        .sin_port = htons(2020)
    };

    rc = bind(sock, (struct sockaddr *) &addr, sizeof(addr));

    if(rc == -1){
        perror("bind");
        return 1;
    }

    bool keep_on_handling_clients = true;

    //lliczba stanow x liczba mozliwyh wejsc
    int states[][6] = {
        {1, 4, 5, 3, 5, 4},
        {1, 2, 5, 3, 5, 4},
        {1, 4, 4, 4, 4, 4},
        {4, 4, 5, 4, 4, 4}
    };

    unsigned char buf[1025];
    unsigned char word[1025];
    unsigned char result[4 * 2 + 3]; //max to 1024/1024\r\n
    

    while (keep_on_handling_clients)
    {
        int state = 0;
        int words_num = 0;
        int palidromes_num = 0;
        int word_i = 0;
        bool cr_symbol = false;

        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_len = sizeof(clnt_addr);

        cnt = recvfrom(sock, buf, 1025, 0, (struct sockaddr *) &clnt_addr, &clnt_addr_len);

        if (cnt == -1){
            perror("recevefrom");
            return -1;
        }

        if (cnt > 1024){
            cnt = sendto(sock, "ERROR", 5, 0, (struct sockaddr *) &clnt_addr, clnt_addr_len);
            if (cnt == -1){
                perror("recevefrom");
                return -1;
            }
        }
        else{
            buf[cnt] = '\0';

            for (int i = 0; i <= cnt; ++i){
                
                state = get_state(buf + i, state, states, i, cnt);
                //printf("symbol: %u state: %d\n", buf[i], state);
                if (state == 1){
                    if (buf[i] >= 65 && buf[i] <= 90){
                        buf[i] +=  32;
                    }
                    word[word_i] = buf[i];
                    ++word_i;
                }

                if (state == 4){
                    unsigned char error_msg[7] = {"ERROR"};
                    int error_len = 5;
                    if (cnt > 2 && buf[cnt - 2] == '\r' && buf[cnt - 1] == '\n'){
                        error_len += 2;
                        memcpy(error_msg + 5, "\r\n", 2);
                    } else if (cnt > 1 && buf[cnt - 1] == '\n'){
                        error_len += 1;
                        memcpy(error_msg + 5, "\n", 2);
                    }
                    cnt = sendto(sock, error_msg, error_len, 0, (struct sockaddr *) &clnt_addr, clnt_addr_len);
                    if (cnt == -1){
                        perror("recevefrom");
                        return -1;
                    }
                    break;
                }

                if (state == 2){
                    if (is_palidrome(word, word_i)){
                        ++palidromes_num;  
                    }
                    ++words_num;
                    word_i = 0;
                }

                if (state == 3){
                    cr_symbol = true;
                }

                if (state == 5){

                    if (word_i > 0){
                        if (is_palidrome(word, word_i)){
                            ++palidromes_num;  
                        }
                        ++words_num;
                    }

                    int len = sprintf(result, "%d", palidromes_num);
                    result[len] = '/';
                    ++len;
                    len += sprintf(result + len, "%d", words_num);
                    //zwraca len nie biorac pod uwage '/0'

                    if (cr_symbol){
                        memcpy(result + len, "\r\n", 2);
                        len += 2;
                    }
                    else if (buf[i] == '\n'){
                        result[len] = '\n';
                        ++len;
                    }
                    
                    cnt = sendto(sock, result, len, 0, (struct sockaddr *) &clnt_addr, clnt_addr_len);
                    if (cnt == -1){
                        perror("recevefrom");
                        return -1;
                    }

                    break;
                }
            } 
        }
    }

    rc = close(sock);

    if(rc == -1){
        perror("close");
        return 1;
    }
    
    return 0;
}
