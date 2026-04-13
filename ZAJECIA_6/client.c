#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int	main(int argc, char **argv)
{

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd == -1){
        perror("socket");
        return 1;
    }
    int rc;

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = htonl(0x7F000001)},
        .sin_port = htons(2020)
    };

    rc = connect(sock_fd, (struct sockaddr *) & server_addr, sizeof(server_addr));
    if (rc == -1){
        perror("connect");
        return 1;
    }

    //const char message[] = "Ala ma kota\r\nAla ma kota\r\noko\r\nolo olo ol";
    char message [2049];
    for (int i = 0; i < 2047; ++ i){
        
        if (i % 2 == 1){
            message[i] = ' ';
        }
        else{
            message[i] = 'i';
        }
    }
    message[2047] = '\r';
    message[2048] = '\n';

    char message2[] = "kk\0 olo olo ok ok ok olo\r\niooi olo. Ala\r\n\r\n \r\n";
    
  
    
    int amount_data_to_send = sizeof(message) - 1;
    /*while (amount_data_to_send > 0)
    {
        int bytes_written = write(sock_fd, message, sizeof(message) - 1);
        if (bytes_written == -1){
            perror("write");
            return 1;
        }
        amount_data_to_send -= bytes_written;
        printf("hoho\n");
    }*/

    int bytes_written = write(sock_fd, message, sizeof(message));

    sleep(10);
    bytes_written = write(sock_fd, message2, sizeof(message2) - 1);

    
    char buf[1024];
    int t = 0;
    while (t < 5)
    {
        rc = read(sock_fd, buf, sizeof(buf));
        printf("rc %d\n", rc);
    
        for (size_t i = 0; i < rc; i++)
        {
            printf("%c", buf[i]);
        }
        printf("\n");
        sleep(3);
        t += 1;
    }
    
    sleep(10);
    close(sock_fd);
    sleep(5);
    
    
    //

    

    return 0;
}
