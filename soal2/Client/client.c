#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>
#define PORT 8080

int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0;
    struct sockaddr_in serv_addr;
    // Message for Server
    char answer[1024];
    // Message from Server
    char buff[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  // Saat gagal menyambung ke Server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    // Login dan Register
    do {
        memset(buff, 0, sizeof(buff));
        read(sock, buff, sizeof(buff));
        printf("Please select register or login: ");
        scanf("%s", answer);
        send(sock, answer, strlen(answer), 0);
        memset(answer, 0, sizeof(answer));

        // Username
        memset(buff, 0, sizeof(buff));
        read(sock, buff, sizeof(buff));
        printf("username: ");
        scanf("%s", answer);
        send(sock, answer, strlen(answer), 0);
        memset(answer, 0, sizeof(answer));
        // Password
        memset(buff, 0, sizeof(buff));
        read(sock, buff, sizeof(buff));
        printf("password: ");
        scanf("%s", answer);
        send(sock, answer, strlen(answer), 0);
        memset(answer, 0, sizeof(answer));

        // Gagal atau tidak
        read(sock, buff, 1024);
        printf("%s\n", buff);
    } while (strcmp(buff, "login success") != 0);

    printf("\n");
    return 0;
}
