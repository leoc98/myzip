#include "zipHandler.h"
#define __TEST_FILENAME_SUFFIX__
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
    fseek(f, 0, SEEK_SET);
    return true;
}

string UnZip::unzip_without_password() {
    char revSuffix[] = "AYILLI.";
    char* moveRevSuffix = revSuffix;

    // 检查后缀名是否正确
    for (auto rit = path.rbegin(); rit != path.rend() && *moveRevSuffix != '\0';
         rit++) {
        if (*rit != *(moveRevSuffix++)) {
            err = UNZIP_FILE_TYPE_ERROR;
            return "";
        }
    }

    // // 如果最后结束时指针不在\0处，说明未遍历完成
    // if (*moveRevSuffix != '\0') {
    //     err = UNZIP_FILE_TYPE_ERROR;
    //     return "";
    // }

    // 逐步还原所有数据
    // 第一个64位为无符号的整形，记录所有bit数
    // 紧接着的16位为无符号的整形，记录所有的组数量，最多有2^8=256组
    // 后面8+32bit为一组，组的数量参考上行，前8bit为key，后32bit为该key出现次数，用于还原霍夫曼树（未使用的key会被忽略）
    // 最后为数据，对齐的部份可以不用考虑

    // 跳过第0个64bit
    uint64_t skip;
    fread(&skip, sizeof(uint64_t), 1, f);

    uint64_t totalLen;
    uint16_t cataSize;
    fread(&totalLen, sizeof(uint64_t), 1, f);
    fread(&cataSize, sizeof(uint16_t), 1, f);

    map<uint8_t, uint32_t> cntMap;

    while (cataSize-- > 0) {
        uint8_t key;
        uint32_t cnt;
        fread(&key, sizeof(uint8_t), 1, f);
        fread(&cnt, sizeof(uint32_t), 1, f);
        cntMap[key] = cnt;
    }

    // 构造用于解压的逆转树
    HalfmanTree ht(cntMap);
    ht.formReverseDir();

    string restoreName(path.begin(), path.begin() + path.size() - 7);

#ifdef __TEST_FILENAME_SUFFIX__
    restoreName += ".test";
    cout<<"you are running a test program\n the filename is "<<restoreName<<endl;
#endif

    FILE* fres = fopen(restoreName.c_str(), "wb");
    // FILE* fbug = fopen("testPlus.txt", "rb");
    int bugcnt = 0;

    uint64_t cntBit = 0;
    // string tempSpace[8] = {0};
    string key;
    while (cntBit < totalLen) {
        int remain = totalLen - cntBit >= 8 ? 8 : totalLen - cntBit;
        cntBit += remain;
        uint8_t ch;
        fread(&ch, sizeof(uint8_t), 1, f);
        for (int i = 0; i < remain; i++) {
            key += ch & (1 << i) ? "1" : "0";
            bool exist;
            uint8_t val = ht.revTranslate(key, &exist);
            if (exist) {
                key.clear();
                fwrite(&val, sizeof(uint8_t), 1, fres);
            }
        }
    }

    // if (cntBit != totalLen) {
    //     cout << "没有将所有的bit转化！！！" << endl;
    // }

    fclose(fres);
    remove(path.c_str());
    return restoreName;
}

string UnZip::unzip_with_password() {
    string tempName = path + ".temp";

    FILE* ftmp = fopen(tempName.c_str(), "wb");

    // 跳过第0个64bit
    uint64_t skip;
    fread(&skip, sizeof(uint64_t), 1, f);

    fwrite(&skip, sizeof(uint64_t), 1, ftmp);

    // 用迭代的散列码进行解密
    uint8_t hashKey = MyHash::myHash(password);
    uint8_t ch;
    while (fread(&ch, sizeof(uint8_t), 1, f)) {
        ch ^= hashKey;
        hashKey = MyHash::myHashIt(hashKey);
        fwrite(&ch, sizeof(uint8_t), 1, ftmp);
    }

    // 交换指针，进行无密码解压
    fclose(f);
    fclose(ftmp);
    rename(tempName.c_str(), path.c_str());
    f = fopen(path.c_str(), "rb");
    // fseek(f, 0, SEEK_SET);
    // return "";
    return unzip_without_password();
}