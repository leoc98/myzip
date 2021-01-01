#include "filetrans.h"

string Receiver::mylisten() {
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[4096];
    int n;
    bool flag = false;
    string msg;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return "1";
    }
    printf("----init socket----\n");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    //设置端口可重用
    int contain;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &contain, sizeof(int));

    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
        return "2";
    }
    printf("----bind sucess----\n");

    if (listen(listenfd, 10) == -1) {
        printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
        return "3";
    }

    printf("======listening on port %d=====\n", port);
    while (!flag) {
        struct sockaddr_in client_addr;
        socklen_t size = sizeof(client_addr);
        if ((connfd = accept(listenfd, (struct sockaddr*)&client_addr,
                             &size)) == -1) {
            printf("accept socket error: %s(errno: %d)", strerror(errno),
                   errno);
            continue;
        }
        while (1) {
            n = read(connfd, buff, MAXLINE);
            if (n == 0)
                break;
            msg = buff;
            buff[n] = '\0';
            msg += '\0';
            flag = true;
        }
        printf("recv msg from client: %s\n", msg.c_str());
        close(connfd);
    }
    close(listenfd);
    return msg;
}

bool Sender::send_msg(string msg) {
    int sockfd, len;
    struct sockaddr_in servaddr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &servaddr.sin_addr) <= 0) {
        printf("inet_pton error for %s\n", host.c_str());
        return 1;
    }

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
        return 1;
    }

    char end = '\0';
    
    write(sockfd, msg.c_str(), msg.size()+1);
    // write(sockfd, &end, 1);
    // printf("send msg: %s,write %d B\n",msg.c_str(),sc);
    close(sockfd);

    return 0;
}

bool Receiver::rec_packup(string recURL) {
    if (receiveFile(port, recURL))
        return false;
    return true;
}

bool Sender::send_packup(string sendURL, string storeName) {
    if (storeName.size())
        send_msg(storeName);
    sleep(2);
    if (sendFile(host, port, sendURL))
        return false;
    return true;
}

int sendFile(string host, int port, string url) {
    int sockfd, len;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;
    FILE* fq;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &servaddr.sin_addr) <= 0) {
        printf("inet_pton error for %s\n", host.c_str());
        return 0;
    }

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }
    if ((fq = fopen(url.c_str(), "rb")) == NULL) {
        printf("File open.\n");
        close(sockfd);
        exit(1);
    }

    bzero(buffer, sizeof(buffer));
    while (!feof(fq)) {
        len = fread(buffer, 1, sizeof(buffer), fq);
        if (len != write(sockfd, buffer, len)) {
            printf("write.\n");
            break;
        }
    }
    close(sockfd);
    fclose(fq);

    return 0;
}

int receiveFile(int port, string url) {
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[4096];
    FILE* fp;
    int n;
    bool flag = false;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 1;
    }
    printf("----init socket----\n");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    //设置端口可重用
    int contain;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &contain, sizeof(int));

    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 1;
    }
    printf("----bind sucess----\n");

    if (listen(listenfd, 10) == -1) {
        printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 1;
    }
    if ((fp = fopen(url.c_str(), "wb")) == NULL) {
        printf("File.\n");
        close(listenfd);
        exit(1);
    }

    printf("======waiting for client's request======\n");

    struct sockaddr_in client_addr;
    socklen_t size = sizeof(client_addr);
    if ((connfd = accept(listenfd, (struct sockaddr*)&client_addr, &size)) ==
        -1) {
        printf("accept socket error: %s(errno: %d)", strerror(errno), errno);
        return 1;
    }
    while (1) {
        n = read(connfd, buff, MAXLINE);
        if (n == 0)
            break;
        fwrite(buff, 1, n, fp);
    }
    close(connfd);
    fclose(fp);

    close(listenfd);
    return 0;
}