#include "zipHandler.h"

string UnZip::unzip() {
    // 如果返回的是空字串，说明解压失败
    if (password.size() == 0)
        return unzip_without_password();
    return unzip_with_password();
}

string UnZip::unzip_with_password(){
    bool passwordCheck();
}