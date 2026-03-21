#include <stdbool.h>
#include <stdio.h>

bool is_printable_buf(const void * buf, int len){
    char * buffor = (char *) buf;
    
    for (int i = 0; i < len; ++i){
        if (!(*buffor >= 32 && *buffor <= 126))
            return false;
        ++buffor;
    }
    return true;
}

int main(int argc, char const *argv[])
{
    char tablica_1[3] = {'A', 'B', 'C'};
    printf("%d\n", is_printable_buf(tablica_1, 3));

    char tablica_2[3] = {'\31', 'B', 'C'};
    printf("%d\n", is_printable_buf(tablica_2, 3));
    return 0;
}
