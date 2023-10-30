// 開始
#include <sys/types.h>  /* connect, inet_addr, read, socket, write */
#include <sys/socket.h> /* connect, inet_addr, shutdown, socket */
#include <stdio.h>      /* fprintf, perror */
#include <stdlib.h>     /* exit */
#include <string.h>     /* memset */
#include <netinet/in.h> /* inet_addr */
#include <arpa/inet.h>  /* htons, inet_addr */
#include <sys/uio.h>    /* read, write */
#include <unistd.h>     /* close, read, write */

#define SERVER_ADDR     "127.0.0.1"
#define SERVER_PORT     12345

int main(void) {
    int s, cc;
    struct sockaddr_in sa;
    char buf[1024];

    // ソケット生成 socket()
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // 接続先の名前作成
    memset(&sa, 0, sizeof(sa)); // memsetって何
    sa.sin_len = sizeof(sa); // sin_lenがある場合
    sa.sin_family = AF_INET;
    sa.sin_port = htons(SERVER_PORT);
    sa.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    // ソケット接続要求 connect() : サーバ側プログラムに接続したい
    fprintf(stderr, "Connecting to the server...\n");
    if (connect(s, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
        // 接続できなかったとき
        perror("connect");
        exit(1);
    }
    fprintf(stderr, "Connected.\n"); // 接続できたときの表示

    // サーバのデータを画面に表示
    fprintf (stderr, "Message from the server:\n\n");
    while (1) { // 無限ループ
        int i, maxfd;
        fd_set readfds;

        FD_ZERO(&readfds); // リセット
        FD_SET(s, &readfds); // サーバ見張り
        FD_SET(STDIN_FILENO, &readfds); // 標準入力見張り
        maxfd = s;

        // 準備できるまで待つ
        if (select(maxfd+1, &readfds, NULL, NULL, NULL) < 0) {
            perror("select");
            exit(1);
        }

        if (FD_ISSET(s, &readfds)) {
            cc = read(s, buf, sizeof(buf));
            if (cc <= 0) break;
            if (cc == EOF) break;
            write(1, buf, cc); // サーバからのデータを表示
            fprintf(stderr,"A");
        }

        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            cc = read(STDIN_FILENO, buf, sizeof(buf));
            if (cc <= 0) break;
            if (cc == EOF) break;
            write(s, buf, cc); // 標準入力があったら 内容をサーバへ送信
            fprintf(stderr,"B");
        }
        
        fprintf(stderr, "\n\nFinished receiving.\n");
    }

    if (close(s) == -1) {
        perror("close");
        exit(1);
    }
    printf("finish!!");
    return 0;
}
// 終了
