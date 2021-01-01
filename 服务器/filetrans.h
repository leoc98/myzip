#ifndef __FILETRANS__
#define __FILETRANS__

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#define MAXLINE 4096
using namespace std;
#define SEND_YOU_PACKUP "send you a pack up"
#define SEND_ME_PACKUP "send me a pack up"
int sendFile(string host, int port,string url);
int receiveFile(int port,string url);

class Sock {
   protected:
    string host;
    int port;

   public:
    Sock(string host, int port) : host(host), port(port){};
};

class Sender : Sock {
   public:
    Sender(string host, int port) : Sock(host, port){};
    bool send_packup(string sendURL,string storeName);
    // void send_restore(string receiveURL);
    bool send_msg(string msg);
};

class Receiver : Sock {
   public:
    Receiver(string host, int port) : Sock(host, port){};
    bool rec_packup(string recURL);
    // bool rec_restore(string receiveURL);
    string mylisten();
};
#endif