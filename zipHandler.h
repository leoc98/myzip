#ifndef __ZIPHANDLER__

#define __ZIPHANDLER__
#include <inttypes.h>
#include <stdio.h>
#include <map>
#include <string>
#include "buffer.h"
#include "halfmanTree.h"
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

class ZipBase {
   protected:
    string path;
    FILE* f;
    string password;
    void init() { f = fopen(path.c_str(), "r"); }

   public:
    ZipBase(string path, string password) : password(password), path(path) {
        init();
    }
    ~ZipBase() { fclose(f); }
};

class Zip : public ZipBase {
   public:
    Zip(string path, string password) : ZipBase(path, password) {}
    string zip();
    string zip_with_password();
    string zip_without_password();
    // ~Zip() { fclose(f); }
};

class UnZip : public ZipBase {
//    private:

   public:
    bool passwordCheck();
    UnZip(string path, string password) : ZipBase(path, password) {}
    string unzip();
    string unzip_with_password();
    string unzip_without_password();
};

#endif