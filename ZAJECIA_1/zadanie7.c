#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
//./zad7 tekst.txt z7.txt

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        printf("Program naley uruchomic z dwoma argumentami, bedacymi nazwami plikow");
        return 1;
    }

    int file_1 = open(argv[1], O_RDONLY);               // z tego pliku czytamy
    int file_2 = open(argv[2], O_RDWR | O_CREAT, 0666); // do tego pliku wpisujemy

    if (file_1 == -1)
    {
        perror("File 1 ERROR");
        return 1;
    }
    if (file_2 == -1)
    {
        perror("File 2: ERROR");
        if (close(file_1) == -1)
        {
            perror("CLOSE ERROR File 1");
        }
        return 1;
    }

    int input_size = -1;
    int written = 0;
    char buf[10];
    int line_number = 1;
    int buf_size = 10;
    int line_size = 0;
    char line_buf[10];
    while (input_size)
    {

        input_size = read(file_1, buf, buf_size);

        if (input_size == -1)
        {
            perror("READ ERROR");
            return 1;
        }

 

        line_size = 0;
        char *line_buf_iter = line_buf;
        for (int i = 0; i < input_size; ++i){
            if (buf[i] == '\n')
                ++ line_number;
            if (line_number % 2 == 1){
                *line_buf_iter = buf[i];
                ++line_buf_iter;
                ++line_size;
            }
        }

        written = write(file_2, line_buf, line_size);

        if (written == -1)
        {
            perror("WRITE ERROR");
            return 1;
        }
    }

    if (close(file_1) == -1)
    {
        perror("CLOSE FILE 1 ERROR");
    }

    if (close(file_2) == -1)
    {
        perror("CLOSE FILE 2 ERROR");
    }

    return 0;
}
