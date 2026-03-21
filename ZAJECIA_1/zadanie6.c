#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if (argc < 3){
        printf("Program naley uruchomic z dwoma argumentami, bedacymi nazwami plikow");
        return 1;
    }
    //printf("%s", argv[1]);
    //printf("%s", argv[2]);

    int file_1 = open(argv[1], O_RDONLY); //z tego pliku czytamy
    int file_2 = open(argv[2], O_RDWR | O_CREAT, 0666); //do tego pliku wpisujemy
    //printf("%d", file_1);

    if (file_1 == -1){
        perror("File 1 ERROR");
        return 1;
    }
    if (file_2 == -1){
        perror("File 2: ERROR");
        if (close(file_1) == -1) {
            perror("CLOSE ERROR File 1");
        }
        return 1;
    }

    int input_size = -1;
    int written = 0;
    char buf[10];
    while (input_size){
        
        input_size = read(file_1, buf, 10);

        if(input_size == -1){
            perror("READ ERROR");
            return 1;
        }
        written = write(file_2, buf, input_size);

        if(written == -1){
            perror("WRITE ERROR");
            return 1;
        }
    }

    if (close(file_1) == -1){
        perror("CLOSE FILE 1 ERROR");
    }

    if(close(file_2) == -1){
        perror("CLOSE FILE 2 ERROR");
    }
    

    return 0;
}
