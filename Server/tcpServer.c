#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORTNUM 0x9190
#define BUFSIZE 256

int main(void) {
    struct sockaddr_in serv_addr, cli_addr;
    int serv_fd, cli_fd, clilen;
    char buffer[BUFSIZE];

    printf("[TCP server for chatting...]\n");

    if ((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("ERROR opening socket");
        exit(1);
    }

    memset((char *)&serv_addr, 0x00, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORTNUM);

    if (bind(serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("ERROR on binding");
        exit(1);
    }

    listen(serv_fd, 5);

    loop:
    clilen = sizeof(cli_addr);
    if ((cli_fd = accept(serv_fd, (struct sockaddr *)&cli_addr, &clilen)) == -1) {
        perror("ERROR on accept");
        exit(1);
    }

    write(cli_fd, "Welcome to Chat Server...", BUFSIZE);

    while(1) {
        memset(buffer, 0x00, sizeof(buffer));

        if((read(cli_fd, buffer, BUFSIZE)) == -1) {
            perror("ERROR reading from socket");
            exit(1);
        }

        printf("[Guest] %s\n", buffer);

        if (buffer[0] == 'q') {
            close(cli_fd);
            goto loop;
        }

        memset(buffer, 0x00, sizeof(buffer));
        printf("[Server] ");
        fgets(buffer, BUFSIZE, stdin);

        write(cli_fd, buffer, BUFSIZE);
        if (buffer[0] == 'q') {
            break;
        }
    }

    close(serv_fd);
    return 0;
}
