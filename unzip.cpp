#include "zipHandler.h"

string UnZip::unzip() {
    // 如果返回的是空字串，说明解压失败
    // if (password.size() == 0)
    //     return unzip_without_password();
    return unzip_with_password();
}

bool UnZip::passwordCheck() {
    uint8_t hashKey = MyHash::myHash(password);
    uint64_t randomHead;
    const char* randomHeadOrigin = RANDOM_HEAD;
    if (fread(&randomHead, sizeof(uint64_t), 1, f) == 0) {
        fseek(f, 0, SEEK_SET);
        return false;
    }

    uint8_t* prandomHead8 = (uint8_t*)&randomHead;
    for (int i = 0; i * 8 < RANDOM_HEAD_SIZE; i++) {
        int a = prandomHead8[i] ^ hashKey;
        int b = randomHeadOrigin[i];
        if (a != b) {
            return false;
        }
    }

    return true;
}

string UnZip::unzip_with_password() {
    bool passwordCheck();
    return "";
}