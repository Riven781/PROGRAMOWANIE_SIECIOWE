#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/syscall.h>
#define MAX_EVENTS 15
#define CLIENTS_ARRAY_LEN 1000

using namespace std;

int add_fd_to_epoll(int fd, int epoll_fd){
    struct epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    int rv = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    if (rv == -1){
        cout << "epoll_ctl(ADD)" << strerror(errno) << endl;
    }
    return rv;
}

int remove_fd_from_epoll(int fd, int epoll_fd){
    int rv = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
    if (rv == -1){
        cout << "epoll_ctl(DEL)" << strerror(errno) << endl;
    }
    return rv;
}

//zmodyfikuj ustawienia powiazane z fd (glownie uzywana aby dodac/usunac EPOLLOUT)
int change_fd_from_epoll(int fd, int epoll_fd, struct epoll_event *ev){
    int rv = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, ev);
    if (rv == -1){
        cout << "epoll_ctl(MOD)" << strerror(errno) << endl;
    }
    return rv;
}

class ClientState
{
public:
    int fd;
    int state;
    int words_num;
    int palidromes_num;
    bool is_error;
    vector<char> word;
    ClientState * next_client;
    ClientState(int fd) : fd{fd}, state{}, words_num{}, palidromes_num{}, next_client{}, word{}, is_error{} {};
    void reset();
    ~ClientState() {};
};

void ClientState::reset(){
    words_num = 0;
    palidromes_num = 0;
    is_error = 0;
    word.clear();
}


ClientState * client_state_array[CLIENTS_ARRAY_LEN]{};

void create_client_state(int fd){
    ClientState * client_state = new ClientState(fd);

    int index = fd % CLIENTS_ARRAY_LEN;

    if (client_state_array[index] == nullptr){
        client_state_array[index] = client_state;
    }
    else{
        ClientState * c_iter = client_state_array[index];
        while (c_iter->next_client != nullptr){
            c_iter = c_iter->next_client;
        }
        c_iter->next_client = client_state;

    }
}

ClientState * get_client_state(int fd){
    int index = fd % CLIENTS_ARRAY_LEN;

    if (client_state_array[index]->fd == fd){
        return client_state_array[index];
    }
    else{
        ClientState * c_iter = client_state_array[index];
        while (c_iter->fd != fd){
            c_iter = c_iter->next_client;
        }
        return c_iter;
    }
}

void delete_client_state(int fd){
    int index = fd % CLIENTS_ARRAY_LEN;
    ClientState * client_to_remove = client_state_array[index];
    ClientState * prev{};
    while (client_to_remove -> fd != fd){
        prev = client_to_remove;
        client_to_remove = client_to_remove->next_client;
    }

    ClientState * next_client = client_to_remove->next_client;
    if (prev == nullptr){
        client_state_array[index] = next_client;
    }
    else{
        prev->next_client = next_client;
    }

    delete client_to_remove;
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

bool is_palidrome(const vector<char>& word){
    for (int i = 0; i < word.size() / 2; ++i){
        if (word[i] != word[word.size() - 1 - i]){
            return false;
        }
    }
    return true;
}

int main(int argc, char const *argv[])
{
    int srv_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (srv_sock == -1){
        cout << "Socket error" << strerror(errno) << endl;
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(2020),
        .sin_addr = { .s_addr = htonl(INADDR_ANY)}
    };

    if(bind(srv_sock, (struct sockaddr *) & addr, sizeof(addr)) == -1){
        cout << "bind error" << strerror(errno) << endl;
        return 1;
    }

    if(listen(srv_sock, 1000) == -1){
        cout << "bind error" << strerror(errno) << endl;
        return 1;
    }

    
    int epoll_fd = epoll_create(10);
    if (epoll_fd == -1){
        cout << "epoll_create" << strerror(errno) << endl;
        return 1;
    }

    if (add_fd_to_epoll(srv_sock, epoll_fd) == -1){
        //zamkniej epoll przec close
        return 1;
    }
    int rc;
    while (true)
    {
        struct epoll_event events[MAX_EVENTS];

        int num = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); //czekanie w nieskonczonosc na gniazdka od ktorych mozna czytac

        if (num == -1){
            cout << "epoll_wait" << strerror(errno) << endl;
            return 1;
        }

        for (int i = 0; i < num; ++i){
            int fd = events[i].data.fd;

            if (events[i].events & EPOLLHUP || events[i].events & EPOLLERR){
                std::cout << "bye: " << fd << std::endl;
                delete_client_state(fd);
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
                    cout << "accept_error" << strerror(errno) << endl;
                    return 1;
                }
                if(add_fd_to_epoll(s, epoll_fd) == -1){
                    return 1;
                }

                create_client_state(s);
                printf("Client accepted: %d\n", s);
            } else {
                printf("Client action: %d\n", fd);
                ClientState * client_state = get_client_state(fd);

                unsigned char buf[1024];

                int cnt = read(fd, buf, sizeof(buf));

                if (cnt == -1){
                    cout << "read error" << strerror(errno) << endl;
                    return 1;
                }
                else if (cnt == 0){
                    delete_client_state(fd);
                    close(fd);
                    break;
                }
                else{
                    for (int i = 0; i < cnt; ++i){
                        int state = get_state(client_state->state, buf[i]);
                        client_state->state = state;

                        if (state == 1){
                            if (buf[i] >= 'A' && buf[i] <= 'Z'){
                                buf[i] += 32;
                            }
                            client_state->word.push_back(buf[i]);
                        }
                        else if (state == 2){
                            ++client_state->words_num;
                            if (is_palidrome(client_state->word)){
                                ++client_state->palidromes_num;
                            }
                            client_state->word.clear();
                        }
                        else if (state == 4){
                            if (client_state->word.size() > 0){
                                ++client_state->words_num;
                                if (is_palidrome(client_state->word)){
                                    ++client_state->palidromes_num;
                                }
                                client_state->word.clear();
                            }

                            string msg;

                            if (client_state->is_error){
                                msg = "ERROR\r\n";
                            } else{
                                msg = to_string(client_state->palidromes_num) + "/" + to_string(client_state->words_num) + "\r\n";
                                
                            }
                            int amount_data_to_send = msg.size();
                            while (amount_data_to_send > 0){
                                ssize_t bytes_written = write(fd, msg.c_str(), msg.size());
                                if(bytes_written == -1){
                                    cout << "write error" << strerror(errno) << endl;
                                    return 1;
                                }
                                amount_data_to_send -= bytes_written;
                            }

                            client_state->reset();
                        }
                        else if (state == 5){
                            client_state->is_error = true;
                        }
                        else if (state == 6){  //klient przyslal dane inne niz ASCII
                            delete_client_state(fd);
                            rc = close(fd);
                            if (rc == -1){
                                cout << "close error" << strerror(errno) << endl;
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
