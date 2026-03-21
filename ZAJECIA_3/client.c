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
int test(int sock, int (*do_test)(unsigned char *), struct sockaddr * addr){
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



    test(sock, test1, (struct sockaddr *) &addr);
    test(sock, test2, (struct sockaddr *) &addr);
    test(sock, test3, (struct sockaddr *) &addr);
    test(sock, test4, (struct sockaddr *) &addr);
    test(sock, test5, (struct sockaddr *) &addr);
    test(sock, test6, (struct sockaddr *) &addr);
    test(sock, test7, (struct sockaddr *) &addr);
    test(sock, test8, (struct sockaddr *) &addr);
    test(sock, test9, (struct sockaddr *) &addr);
    test(sock, test10, (struct sockaddr *) &addr);
    test(sock, test11, (struct sockaddr *) &addr);

    test(sock, test12, (struct sockaddr *) &addr);
    test(sock, test13, (struct sockaddr *) &addr);
    test(sock, test14, (struct sockaddr *) &addr);
    test(sock, test15, (struct sockaddr *) &addr);
    test(sock, test16, (struct sockaddr *) &addr);
    test(sock, test17, (struct sockaddr *) &addr);
    test(sock, test18, (struct sockaddr *) &addr);

    test(sock, test19, (struct sockaddr *) &addr);
    test(sock, test20, (struct sockaddr *) &addr);
    test(sock, test21, (struct sockaddr *) &addr);
    test(sock, test22, (struct sockaddr *) &addr);

    test(sock, test23, (struct sockaddr *) &addr);
    test(sock, test24, (struct sockaddr *) &addr);

    test(sock, test25, (struct sockaddr *) &addr);
    test(sock, test26, (struct sockaddr *) &addr);
    test(sock, test27, (struct sockaddr *) &addr);



   

    rc = close(sock);

    if(rc == -1){
        perror("close");
        return 1;
    }

    return 0;
}

