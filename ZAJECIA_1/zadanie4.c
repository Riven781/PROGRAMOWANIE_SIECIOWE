#include <stdbool.h>
#include <stdio.h>

bool is_printable_str(const char * str){
    while (*str != '\0'){
        if (!(*str >= 32 && *str <= 126))
            return false;
        ++str;
    }
    return true;
}

int main(int argc, char const *argv[])
{
    char tablica_1[] = "Super";
    printf("%d\n", is_printable_str(tablica_1));

    char tablica_2[] = "Sup\31r";
    printf("%d\n", is_printable_str(tablica_2));
    return 0;
}
