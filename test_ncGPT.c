#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        return 1;
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];

    // サーバーソケットを作成
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    // サーバーアドレスを設定
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    // サーバーに接続
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    while (1) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(sockfd, &read_fds);

        int max_fd = sockfd > STDIN_FILENO ? sockfd : STDIN_FILENO;

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            // 標準入力から読み込み
            int bytes_read = read(STDIN_FILENO, buffer, MAX_BUFFER_SIZE);
            if (bytes_read <= 0) {
                break;
            }
            // サーバーに送信
            send(sockfd, buffer, bytes_read, 0);
        }

        if (FD_ISSET(sockfd, &read_fds)) {
            // サーバーから読み込み
            int bytes_received = recv(sockfd, buffer, MAX_BUFFER_SIZE, 0);
            if (bytes_received <= 0) {
                break;
            }
            // 標準出力に出力
            write(STDOUT_FILENO, buffer, bytes_received);
        }
    }

    close(sockfd);
    return 0;
}
