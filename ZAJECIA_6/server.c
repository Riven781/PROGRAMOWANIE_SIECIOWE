#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/syscall.h>
#define MAX_EVENTS 15

struct ClientState
{
    int fd;
    int state;
    int words_num;
    int palidromes_num;
    bool is_error;
    char word[1024];
    int word_i;
};

void reset(struct ClientState * client_state){
    client_state->is_error = false;
    client_state->palidromes_num = 0;
    client_state->words_num = 0;
    client_state->word_i = 0;
}

struct ClientState* create_client_state(int fd){
    struct ClientState * client_state = (struct ClientState *)malloc(sizeof(struct ClientState));
    reset(client_state);
    client_state->fd = fd;
    client_state->state = 0;
    return client_state;
}

int add_fd_to_epoll(int fd, int epoll_fd){
    struct epoll_event ev = {0};
    ev.events = EPOLLIN;
    struct ClientState * client_state = create_client_state(fd);
    if (client_state == NULL){
        perror("malloc failed");
        return -1;
    }
    ev.data.ptr = client_state;
    int rv = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    if (rv == -1){
        perror("eppol_ctl(ADD)");
    }
    return rv;
}

int remove_fd_from_epoll(int fd, int epoll_fd){
    int rv = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
    if (rv == -1){
        perror("eppol_ctl(DEL)");
    }
    return rv;
}

const int states[][6] = {
    {1, 5, 3, 5, 5, 6},
    {1, 2, 3, 5, 5, 6},
    {1, 5, 5, 5, 5, 6},
    {5, 5, 5, 4, 5, 6},
    {1, 5, 3, 5, 5, 6},
    {5, 5, 3, 5, 5, 6}
};

int get_state(int old_state, unsigned char symbol){
    if ((symbol >= 'A' && symbol <='Z') || (symbol >= 'a' && symbol <= 'z')){
        return states[old_state][0];
    }
    else if (symbol == ' '){
        return states[old_state][1];
    }
    else if (symbol == '\r'){
        return states[old_state][2];
    }
    else if (symbol == '\n'){
        return states[old_state][3];
    }
    else if (symbol >= 0 && symbol <= 127){
        return states[old_state][4];
    }
    else{
        return states[old_state][5];
    }
}

bool is_palidrome(char * word, int len){
    for (int i = 0; i < len / 2; ++i){
        if (word[i] != word[len - 1 - i]){
            return false;
        }
    }
    return true;
}


int main(int argc, char const *argv[])
{
    int srv_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (srv_sock == -1){
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(2020),
        .sin_addr = { .s_addr = htonl(INADDR_ANY)}
    };

    if(bind(srv_sock, (struct sockaddr *) & addr, sizeof(addr)) == -1){
        perror("bind");
        return 1;
    }

    if(listen(srv_sock, 1000) == -1){
        perror("listen");
        return 1;
    }

    
    int epoll_fd = epoll_create(10);
    if (epoll_fd == -1){
        perror("eppol_create");
        return 1;
    }

    if (add_fd_to_epoll(srv_sock, epoll_fd) == -1){
        return 1;
    }
    int rc;
    while (true)
    {
        struct epoll_event events[MAX_EVENTS];

        int num = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); //czekanie w nieskonczonosc na gniazdka od ktorych mozna czytac

        if (num == -1){
            perror("epoll_wait");
            return 1;
        }

        for (int i = 0; i < num; ++i){
            struct ClientState * client_ptr = events[i].data.ptr;
            int fd = client_ptr->fd;

            if (events[i].events & EPOLLHUP || events[i].events & EPOLLERR){ //EPOLLHUP - zamkniecie poloczenia, EPOLLERR - wystapil blad
                //printf("bye: %d", client_ptr->fd);
                remove_fd_from_epoll(fd, epoll_fd);
                free(client_ptr);
                rc = close(fd);
                if (rc == -1){
                    perror("close");
                    return -1;
                }
                continue;
            }

            if (!(events[i].events & EPOLLIN)){
                continue;
            }

            if (fd == srv_sock){
                int s = accept(srv_sock, NULL, NULL);
                if (s == -1){
                    perror("accept");
                    return 1;
                }
                if(add_fd_to_epoll(s, epoll_fd) == -1){
                    return 1;
                }

                //printf("Client accepted: %d\n", s);
            } else {
                


                unsigned char buf[1024];

                int cnt = read(fd, buf, sizeof(buf));
                //printf("Client action: %d cnt: %d\n", fd, cnt);
                if (cnt == -1){
                    perror("read");
                    return 1;
                }
                else if (cnt == 0){
                    remove_fd_from_epoll(fd, epoll_fd);
                    free(client_ptr);
                    rc = close(fd);
                    if (rc == -1){
                        perror("close");
                        return -1;
                    }
                    break;
                }
                else{
                    for (int i = 0; i < cnt; ++i){
                        int state = get_state(client_ptr->state, buf[i]);
                        
                        client_ptr->state = state;
                        if (state == 1){
                            if (buf[i] >= 'A' && buf[i] <= 'Z'){
                                buf[i] += 32;
                            }
                            if (client_ptr->word_i >= 1024){
                                client_ptr->is_error = true; //najwieksze slowo moze miec 1024 znakow ASCII
                            }
                            else{
                                client_ptr->word[client_ptr->word_i] = buf[i];
                                ++client_ptr->word_i; 
                            }
                        }
                        else if (state == 2){
                            ++client_ptr->words_num;
                            if (is_palidrome(client_ptr->word, client_ptr->word_i)){
                                ++client_ptr->palidromes_num;
                            }
                            client_ptr->word_i = 0;
                        }
                        else if (state == 4){
                            if (client_ptr->word_i > 0){
                                ++client_ptr->words_num;
                                if (is_palidrome(client_ptr->word, client_ptr->word_i)){
                                    ++client_ptr->palidromes_num;
                                }
                                client_ptr->word_i = 0;
                            }

                            if (client_ptr->words_num > 1024){ //moze byc maksymalnie 1024 slow
                                client_ptr->is_error = true;
                            }

                            char msg[12]; //max to 1024/1024\r\n
                            int msg_size = 0;

                            if (client_ptr->is_error){
                                msg_size = 7;
                                memcpy(msg, "ERROR\r\n", msg_size);
                            } else{
                                msg_size += sprintf(msg, "%d/", client_ptr->palidromes_num);
                                msg_size += sprintf(msg + msg_size, "%d\r\n", client_ptr->words_num);  
                            }
                            int amount_data_to_send = msg_size;
                            int offset = 0;
                            while (amount_data_to_send > 0){
                                ssize_t bytes_written = write(fd, msg + offset, amount_data_to_send);
                                if(bytes_written == -1){
                                    perror("write");
                                    return 1;
                                }
                                offset += bytes_written;
                                amount_data_to_send -= bytes_written;
                            }

                            reset(client_ptr);
                        }
                        else if (state == 5){
                            client_ptr->is_error = true;
                        }
                        else if (state == 6){  //klient przyslal dane inne niz ASCII
                            remove_fd_from_epoll(fd, epoll_fd);
                            free(client_ptr);
                            rc = close(fd);
                            if (rc == -1){
                                perror("close");
                                return -1;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    
    
    
    return 0;
}
