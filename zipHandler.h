#ifndef __ZIPHANDLER__

#define __ZIPHANDLER__
#include <inttypes.h>
#include <stdio.h>
#include <map>
#include <string>
#include "halfmanTree.h"
#include "buffer.h"
#include "myhash.h"
using namespace std;


class ZipHandler {
   private:
    string path;

   public:
    ZipHandler(string path);
    string zip(string password);
    string unzip(string password);
};

class Zip {
   private:
    string path;
    FILE* f;
    string password;
    void init() { f = fopen(path.c_str(), "r"); }
    // uint8_t myHash(string password);// 用于首次生成哈希
    // uint8_t myHash(uint8_t lastKey);// 用于迭代生成hash

   public:
    Zip(string path, string password) : password(password), path(path) {
        init();
    }
    string zip();
    string zip_with_password();
    string zip_without_password();
    ~Zip() { fclose(f); }
};

class UnZip {
   private:
    bool usePassWord;
    FILE* f;
    string password;

   public:
    UnZip(string path, string password);
    string unzip();
    string unzip_with_password();
    string unzip_without_password();
};

#endif