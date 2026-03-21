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

int test1(unsigned char * buf){
    printf("\n---TEST 1:---\n");
    unsigned char text[] = {"Ala ma kota"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test2(unsigned char * buf){
    printf("\n---TEST 2 (\\n in middle):---\n");
    unsigned char text[] = {"Ala ma\n kota"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test3(unsigned char * buf){
    printf("\n---TEST 3 (empty):---\n");
    return 0;
}

int test4(unsigned char * buf){
    printf("\n---TEST 4 (too large input):---\n");
    for (size_t i = 0; i < 1025; i++)
    {
        buf[i] = 'i';
    }
    return 1025;
}

int test5(unsigned char * buf){
    printf("\n---TEST 5 (perfect input):---\n");
    for (size_t i = 0; i < 1024; i++)
    {
        buf[i] = 'i';
    }
    return 1024;
}

int test6(unsigned char * buf){
    printf("\n---TEST 6 (the most palidromes):---\n");
    for (size_t i = 0; i < 1024; i++)
    {
        if(i % 2 == 0){
            buf[i] = 'i';
        }
        else{
            buf[i] = ' ';
        }
    }
    buf[1023] = 'i';
    return 1024;
}

int test7(unsigned char * buf){
    printf("\n---TEST 7 (first space):---\n");
    unsigned char text[] = {" Ala ma kota"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test8(unsigned char * buf){
    printf("\n---TEST 8 (last space):---\n");
    unsigned char text[] = {"Ala ma kota "};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test9(unsigned char * buf){
    printf("\n---TEST 9 (more spaces in the middle):---\n");
    unsigned char text[] = {"Ala ma  kota"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test10(unsigned char * buf){
    printf("\n---TEST 10 (\\r in the middle):---\n");
    unsigned char text[] = {"Ala m\ra kota"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test11(unsigned char * buf){
    printf("\n---TEST 11 (\\r in the beegining):---\n");
    unsigned char text[] = {"\rAla ma kota"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test12(unsigned char * buf){
    printf("\n---TEST 12 (\\r in the end):---\n");
    unsigned char text[] = {"Ala ma kota\r"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test13(unsigned char * buf){
    printf("\n---TEST 13 (\\n in the end):---\n");
    unsigned char text[] = {"Ala ma kota\n"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test14(unsigned char * buf){
    printf("\n---TEST 14 (\\r\\n in the end):---\n");
    unsigned char text[] = {"Ala ma kota\r\n"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test15(unsigned char * buf){
    printf("\n---TEST 15 (too large input becouse of \\n):---\n");
    for (size_t i = 0; i < 1024; i++)
    {
        buf[i] = 'i';
    }
    buf[1024] = '\n';
    return 1025;
}

int test16(unsigned char * buf){
    printf("\n---TEST 16 (perfect input with \\n):---\n");
    for (size_t i = 0; i < 1023; i++)
    {
        buf[i] = 'i';
    }
    buf[1023] = '\n';
    return 1024;
}

int test17(unsigned char * buf){
    printf("\n---TEST 17 (perfect input with \\r\\n):---\n");
    for (size_t i = 0; i < 1022; i++)
    {
        buf[i] = 'i';
    }
    buf[1022] = '\r';
    buf[1023] = '\n';
    return 1024;
}

int test18(unsigned char * buf){
    printf("\n---TEST 18 (the most palidromes with \\r\\n):---\n");
    for (size_t i = 0; i < 1022; i++)
    {
        if (i % 2 == 0){
            buf[i] = 'i';
        }
        else{
            buf[i] = ' ';
        }
    }
    buf[1021] = 'i';
    buf[1022] = '\r';
    buf[1023] = '\n';
    return 1024;
}

int test19(unsigned char * buf){
    printf("\n---TEST 19 (\\0 at the begining):---\n");
    unsigned char text[] = {"\0Ala ma kota"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}


int test20(unsigned char * buf){
    printf("\n---TEST 20 (\\0 in the middle1):---\n");
    unsigned char text[] = {"Ala ma\0 kota"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test21(unsigned char * buf){   
    printf("\n---TEST 21 (\\0 in the middle2):---\n");
    unsigned char text[] = {"Ala ma \0kota"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}


int test22(unsigned char * buf){   //nie mozna tak konczyc
    printf("\n---TEST 22 (\\0 in the end):---\n");
    unsigned char text[] = {"Ala ma kota"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text);
}

int test23(unsigned char * buf){   
    printf("\n---TEST 23 (big letters):---\n");
    unsigned char text[] = {"Ala OK looL OKko nIe"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test24(unsigned char * buf){   
    printf("\n---TEST 24 (bad letter):---\n");
    unsigned char text[] = {"*"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test25(unsigned char * buf){   
    printf("\n---TEST 25 (bad letter):---\n");
    unsigned char text[] = {"a*"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test26(unsigned char * buf){   
    printf("\n---TEST 26 (bad letter):---\n");
    unsigned char text[] = {"a *"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test27(unsigned char * buf){   
    printf("\n---TEST 27 (bad letter):---\n");
    unsigned char text[] = {"a *t"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test28(unsigned char * buf){
    printf("\n---TEST 28 (only \\n):---\n");
    buf[0] = '\n';
    return 1;
}

int test29(unsigned char * buf){
    printf("\n---TEST 29 (only \\n\\r - bad seq):---\n");
    buf[0] = '\n';
    buf[1] = '\r';
    return 2;
}

int test30(unsigned char * buf){
    printf("\n---TEST 30 (only \\r\\n):---\n");
    buf[0] = '\r';
    buf[1] = '\n';
    return 2;
}

int test31(unsigned char * buf){
    printf("\n---TEST 31 (error with \\n):---\n");
    unsigned char text[] = {"Ala ma/ kota\n"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test32(unsigned char * buf){
    printf("\n---TEST 32 (error with \\r\\n):---\n");
    unsigned char text[] = {"Ala ma/ kota\r\n"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test33(unsigned char * buf){
    printf("\n---TEST 33 (error with \\r\\n len == 3):---\n");
    unsigned char text[] = {"*\r\n"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}


int test34(unsigned char * buf){
    printf("\n---TEST 34 (error with \\n len == 2):---\n");
    unsigned char text[] = {"*\n"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}


int test35(unsigned char * buf){
    printf("\n---TEST 35 (only space):---\n");
    unsigned char text[] = {" "};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test36(unsigned char * buf){
    printf("\n---TEST 36 (only space \\n):---\n");
    unsigned char text[] = {" \n"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test37(unsigned char * buf){
    printf("\n---TEST 37 (only space \\r\\n):---\n");
    unsigned char text[] = {" \r\n"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test38(unsigned char * buf){
    printf("\n---TEST 38:--- (palidromes test)\n");
    unsigned char text[] = {"O KO OKO LOL kajak kajaki pokaz zaz ll"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}
int test39(unsigned char * buf){
    printf("\n---TEST 39:---\n");
    unsigned char text[] = {"O KO OKO LOL kajak kajaki pokaz zaz ll\n"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}
int test40(unsigned char * buf){
    printf("\n---TEST 40:---(space before \\n)\n");
    unsigned char text[] = {"O KO OKO LOL kajak kajaki pokaz zaz ll \n"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test41(unsigned char * buf){
    printf("\n---TEST 41:---(space before \\r\\n)\n");
    unsigned char text[] = {"O KO OKO LOL kajak kajaki pokaz zaz ll \r\n"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test42(unsigned char * buf){
    printf("\n---TEST 42:---(0 palidormes)\n");
    unsigned char text[] = {"koala pilka zero nie ma tu palidromow"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test43(unsigned char * buf){
    printf("\n---TEST 43:---(0 palidormes + \\n)\n");
    unsigned char text[] = {"koala pilka zero nie ma tu palidromow\n"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}

int test44(unsigned char * buf){
    printf("\n---TEST 44:---(0 palidormes + \\r\\n)\n");
    unsigned char text[] = {"koala pilka zero nie ma tu palidromow\r\n"};
    memcpy(buf, text, sizeof(text));
    return sizeof(text) - 1;
}
bool test(int sock, int (*do_test)(unsigned char *), struct sockaddr * addr,int exp_len, unsigned char * exp_result){
    unsigned char buf[2000];
    int size = do_test(buf);

    int cnt = sendto(sock, buf, size , 0, addr, sizeof(*addr));

    if(cnt == -1){
        perror("sendto");
        return 1;
    }

    cnt = recvfrom(sock, buf, 16, 0, NULL, NULL);

    if(cnt == -1){
        perror("recvfrom");
        return 1;
    }
    buf[cnt] = '\0';

    printf("%s", buf);
    printf("Liczba bajtow: %d\n", cnt);

    for (int i = 0; i < cnt; i++)
    {
        printf("%u,", buf[i]);
    }
    printf("\n");

    if (exp_len != -1){
        if (exp_len != cnt){
            printf("!!!!!!!! LEN ERROR !!!!!!!!!\n");
            return false;
        }
        else{
            if(memcmp(exp_result, buf, exp_len)  != 0){
                printf("!!!!!!!! CMP ERROR !!!!!!!!!\n");
                return false;
            }
        }
    }

    return true;

}

int main(int argc, char const *argv[])
{

    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // na wyniki zwracane przez recvfrom() i sendto()

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock == -1){
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = {.s_addr = htonl(0x7F000001)},
        .sin_port = htons(2020)
    };

    unsigned char buf[16];
    //memcpy(buf, "Hello world\r\n", 4);



    test(sock, test1, (struct sockaddr *) &addr , 3, "1/3");
    test(sock, test2, (struct sockaddr *) &addr, 5, "ERROR");
    test(sock, test3, (struct sockaddr *) &addr, 3, "0/0");
    test(sock, test4, (struct sockaddr *) &addr, 5, "ERROR");
    test(sock, test5, (struct sockaddr *) &addr, 3, "1/1");
    test(sock, test6, (struct sockaddr *) &addr, 7, "512/512");
    test(sock, test7, (struct sockaddr *) &addr, 5, "ERROR");
    test(sock, test8, (struct sockaddr *) &addr, 5, "ERROR");
    test(sock, test9, (struct sockaddr *) &addr, 5, "ERROR");
    test(sock, test10, (struct sockaddr *) &addr, 5, "ERROR");
    test(sock, test11, (struct sockaddr *) &addr, 5, "ERROR");

    test(sock, test12, (struct sockaddr *) &addr, 5, "ERROR");
    test(sock, test13, (struct sockaddr *) &addr, 4, "1/3\n");
    test(sock, test14, (struct sockaddr *) &addr, 5, "1/3\r\n");
    test(sock, test15, (struct sockaddr *) &addr, 5, "ERROR");
    test(sock, test16, (struct sockaddr *) &addr, 4, "1/1\n");
    test(sock, test17, (struct sockaddr *) &addr, 5, "1/1\r\n");
    test(sock, test18, (struct sockaddr *) &addr, 9, "511/511\r\n");

    test(sock, test19, (struct sockaddr *) &addr, 5, "ERROR");
    test(sock, test20, (struct sockaddr *) &addr, 5, "ERROR");
    test(sock, test21, (struct sockaddr *) &addr, 5, "ERROR");
    test(sock, test22, (struct sockaddr *) &addr, 5, "ERROR");

    test(sock, test23, (struct sockaddr *) &addr, 3, "3/5");
    test(sock, test24, (struct sockaddr *) &addr, 5, "ERROR");

    test(sock, test25, (struct sockaddr *) &addr, 5, "ERROR");
    test(sock, test26, (struct sockaddr *) &addr, 5, "ERROR");
    test(sock, test27, (struct sockaddr *) &addr, 5, "ERROR");

    test(sock, test28, (struct sockaddr *) &addr, 4, "0/0\n");
    test(sock, test29, (struct sockaddr *) &addr, 5, "ERROR");
    test(sock, test30, (struct sockaddr *) &addr, 5, "0/0\r\n");

    test(sock, test31, (struct sockaddr *) &addr, 6, "ERROR\n");
    test(sock, test32, (struct sockaddr *) &addr, 7, "ERROR\r\n");

    test(sock, test33, (struct sockaddr *) &addr, 7, "ERROR\r\n");
    test(sock, test34, (struct sockaddr *) &addr, 6, "ERROR\n");

    test(sock, test35, (struct sockaddr *) &addr, 5, "ERROR");
    test(sock, test36, (struct sockaddr *) &addr, 6, "ERROR\n");

    test(sock, test37, (struct sockaddr *) &addr, 7, "ERROR\r\n");
    test(sock, test38, (struct sockaddr *) &addr, 3, "6/9");

    test(sock, test39, (struct sockaddr *) &addr, 4, "6/9\n");
    test(sock, test40, (struct sockaddr *) &addr, 6, "ERROR\n");
    test(sock, test41, (struct sockaddr *) &addr, 7, "ERROR\r\n");
    test(sock, test42, (struct sockaddr *) &addr, 3, "0/7");


    test(sock, test43, (struct sockaddr *) &addr, 4, "0/7\n");
    test(sock, test44, (struct sockaddr *) &addr, 5, "0/7\r\n");


    rc = close(sock);

    if(rc == -1){
        perror("close");
        return 1;
    }

    return 0;
}

