#include "zipHandler.h"

string UnZip::unzip() {
    // 如果返回的是空字串，说明解压失败
    if (passwordCheck() == false)
        return "";
    if (password.size() == 0)
        return unzip_without_password();
    return unzip_with_password();
}

bool UnZip::passwordCheck() {
    uint8_t hashKey = MyHash::myHash(password);
    uint64_t randomHead;
    const char* randomHeadOrigin = RANDOM_HEAD;
    if (fread(&randomHead, sizeof(uint64_t), 1, f) == 0) {
        fseek(f, 0, SEEK_SET);
        err = UNZIP_FILE_TOO_SMALL;
        return false;
    }

    uint8_t* prandomHead8 = (uint8_t*)&randomHead;
    for (int i = 0; i * 8 < RANDOM_HEAD_SIZE; i++) {
        int a = prandomHead8[i] ^ hashKey;
        int b = randomHeadOrigin[i];
        if (a != b) {
            fseek(f, 0, SEEK_SET);
            err = INCORRECT_PASSWORD;
            return false;
        }
    }

    return true;
}

string UnZip::unzip_without_password() {
    string restoreName = path;
    char revSuffix[] = "AYILLI.";
    char* moveRevSuffix = revSuffix;

    // 检查后缀名是否正确
    for (auto rit = path.rbegin(); rit != path.rend(); rit++) {
        if (*rit != *(moveRevSuffix++)) {
            err = UNZIP_FILE_TYPE_ERROR;
            return "";
        }
    }

    // 如果最后结束时指针不在\0处，说明未遍历完成
    if (*moveRevSuffix != '\0') {
        err = UNZIP_FILE_TYPE_ERROR;
        return "";
    }

    // 逐步还原所有数据
    // 第一个64位为无符号的整形，记录所有bit数
    // 紧接着的8位为无符号的整形，记录所有的组数量，最多有2^8=256组
    // 后面8+32bit为一组，组的数量参考上行，前8bit为key，后32bit为该key出现次数，用于还原霍夫曼树（未使用的key会被忽略）
    // 最后为数据，对齐的部份可以不用考虑
    uint64_t totalLen;
    uint8_t cataSize;
    fread(&totalLen, sizeof(uint64_t), 1, f);
    fread(&cataSize, sizeof(uint8_t), 1, f);

    map<uint8_t, uint32_t> cntMap;

    while (cataSize-- > 0) {
        uint8_t key;
        uint32_t cnt;
        fread(&key, sizeof(uint8_t), 1, f);
        fread(&cnt, sizeof(uint32_t), 1, f);
        cntMap[key] = cnt;
    }

    return "";
}

string UnZip::unzip_with_password() {
    return "";
}