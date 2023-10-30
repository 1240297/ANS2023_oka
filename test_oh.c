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
    while ((cc = read(s, buf, sizeof(buf))) > 0) // 無限ループ？
        write(1, buf, cc);// 標準入力があったら 内容をサーバへ送信
    if (cc == -1) { // エラー発生
        perror("read");
        exit(1);
    }
    fprintf(stderr, "\n\nFinished receiving.\n");

    if (cc == EOF) {

    }

        // 受信/送信 read()/write() : サーバ側プログラムとデータのやり取り
        
        // サーバからデータを受信したら 標準出力に出力


    }
// 終了