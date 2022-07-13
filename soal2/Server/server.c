#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <math.h>
#define PORT 8080
#define SO_REUSEPORT 15

int n;

struct user {
    char username[50];
    char password[50];
};

void regis(char *username, char *password) {
    FILE *fptr;
    fptr = fopen("users.txt", "a");
    fprintf(fptr, "%s:%s\n", username, password);
    fclose(fptr);
}

int search_user(char *username, char *password) {
    int n = 0, mode = 0;
    struct user users[50];
    int i = 0;
    do {
        memset(users[i].username, 0, sizeof(users[i].username));
        memset(users[i].password, 0, sizeof(users[i].password));
        i++;
    } while (i < 50);
    //Buka file dan baca file users.txt
    FILE *fptr;
    fptr = fopen("users.txt", "r");

    fseek(fptr, 0, SEEK_END);
    long fsize = ftell(fptr);
    rewind(fptr);

    char *str = (char *)malloc(sizeof(char) * (fsize + 1));
    fread(str, 1, fsize, fptr);

    for (int i = 0; i <= fsize; ++i) {
        if (str[i] == ':') {
            mode ^= 1;
            continue;
        }
        if (str[i] == '\n' || str[i] == ' ') {
            mode ^= 1;
            ++n;
            continue;
        }
        if (mode == 0) {
            sprintf(users[n].username, "%s%c", users[n].username, str[i]);
        } else {
            sprintf(users[n].password, "%s%c", users[n].password, str[i]);
        }
    }
    fclose(fptr);

    for (int i = 0; i < n; ++i) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    // Message untuk client
    char *login_success = "Login Success";
    char *login_failed = "Login Failed";
    char *register_success = "Register success";
    char *register_failed = "Sorry register failed, please try again";
    char *file_pattern_not_found = "File pattern doesn't match";
    char *less_than_6_char = "Password less than 6 character, register failed, please try again";
    char *login_or_register = "Selected: ";
    char *username = "username: ";
    char *password = "password: ";
    // Message dari client
    char buff[1024] = {0};
    char user_name[1024];
    char user_password[1024];
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("Accept");
            exit(EXIT_FAILURE);
        }
        while (1) {
            send(new_socket, login_or_register, strlen(login_or_register), 0);
            read(new_socket, buff, 1024);
            printf("user %s\n", buff);

            memset(user_name, 0, sizeof(user_name));
            memset(user_password, 0, sizeof(user_password));

            if (strcmp(buff, "register") == 0) {
                // username
                send(new_socket, username, strlen(username), 0);
                read(new_socket, user_name, 1024);
                printf("username: %s\n", user_name);
                
                // password
                send(new_socket, password, strlen(password), 0);
                read(new_socket, user_password, 1024);

                if(strlen(user_password) < 6){
                    send(new_socket, less_than_6_char, strlen(less_than_6_char), 0);
                }else{
                printf("password: %s\n", user_password);
                regis(user_name, user_password);
                send(new_socket, register_success, strlen(register_success), 0);
                }
            } else if (strcmp(buff, "login") == 0) {
                // username
                send(new_socket, username, strlen(username), 0);
                read(new_socket, user_name, 1024);
                printf("username: %s\n", user_name);
                
                // password
                send(new_socket, password, strlen(password), 0);
                read(new_socket, user_password, 1024);
                printf("password: %s\n", user_password);
                
                if (search_user(user_name, user_password)) {
                    send(new_socket, login_success, strlen(login_success), 0);
                    printf("%s\n", login_success);
                    break;
                } else {
                    send(new_socket, login_failed, strlen(login_failed), 0);
                    printf("%s\n", login_failed);
                }
            }
            memset(buff, 0, sizeof(buff));
            sleep(2);
        }
        printf("\n");
        close(new_socket);
    }
    return 0;
}
