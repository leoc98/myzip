#include "zipHandler.h"

string Zip::zip() {
    if (password.size() == 0)
        return zip_without_password();
    return zip_with_password();
}

string Zip::zip_without_password() {
    /*
        1.先读取源文件生成次数表
        2.使用次数表生成霍夫曼树
        3.将次数表保存到ILLIYA文件中
            具体结构为
            第零个64位是一个参与混淆的固定随机数，该随机数主要用于在解压时判断是否需要密码以及输入密码是否正确
            第一个64位为无符号的整形，记录所有bit数
            紧接着的16位为无符号的整形，记录所有的组数量，最多有2^8=256组
            后面8+32bit为一组，组的数量参考上行，前8bit为key，后32bit为该key出现次数，用于还原霍夫曼树（未使用的key会被忽略）
            最后为数据，对齐的部份可以不用考虑
        4.使用霍夫曼树的映射将源文件映射到BUFFER中，再将BUFFER写入源文件（后半在buffer类中自动实现）
    */

    /*
         注意：在内存中是以小端存储，即逻辑存储格式是
             low 1110 1000 high
         但是如果用uint8_t读出，结果为十进制26，即为
             0001 0111 = 0x17
         所以在读取时，应当读取后将uint8_t给倒转过来才是正确的解压方式
         或者从低位读出位，按序放入string即可。
    */

    // 生成次数表
    map<uint8_t, uint32_t> cntMap;
    uint8_t ch;
    while (fread(&ch, sizeof(uint8_t), 1, f)) {
        // cout<<ch;
        cntMap[ch]++;
    }
    // cout<<endl;

    // 生成霍夫曼树
    HalfmanTree ht(cntMap);

    // 生成压缩文件指针
    string tempPath = path;
    tempPath += ".ILLIYA";
    FILE* ftmp = fopen(tempPath.c_str(), "wb");
    // cout << "the size of this tree is " << totalLen << endl;

    // 生成需要写入文件的数据
    // 第零个64位是一个参与混淆(但是不会迭代混淆）的固定随机数，该随机数主要用于在解压时判断是否需要密码以及输入密码是否正确
    // 第一个64位为无符号的整形，记录所有bit数
    // 紧接着的16位为无符号的整形，记录所有的组数量，最多有2^8=256组
    // 后面8+32bit为一组，组的数量参考上行，前8bit为key，后32bit为该key出现次数，用于还原霍夫曼树（未使用的key会被忽略）
    const char* randomHead = RANDOM_HEAD;
    const uint64_t* randomHead64 = (const uint64_t*)randomHead;
    fwrite(randomHead64, sizeof(uint64_t), 1, ftmp);
    uint64_t totalLen = ht.totalSize();
    uint16_t cataSize = ht.getDirectorySize();
    fwrite(&totalLen, sizeof(uint64_t), 1, ftmp);
    fwrite(&cataSize, sizeof(uint16_t), 1, ftmp);
    for (auto item : ht.cntMap) {
        uint8_t key = item.first;
        uint32_t cnt = item.second;
        fwrite(&key, sizeof(uint8_t), 1, ftmp);
        fwrite(&cnt, sizeof(uint32_t), 1, ftmp);
    }

    // 最后为数据，对齐的部份可以不用考虑
    fseek(f, 0, SEEK_SET);
    Buffer buf(ftmp);
    uint8_t& key = ch;

    uint64_t bitCnt = 0;

    while (fread(&key, sizeof(uint8_t), 1, f)) {
        // cout<<key;

        HalfmanValue* hv = ht.translate(key);
        uint8_t* data = hv->value;
        int bitLength = hv->bitLength;
        bitCnt += bitLength;
        buf.fill(data, bitLength);
    }

    // cout<<endl;

    cout << "actual wrote in bit number is " << bitCnt << endl;
    cout << "the written total bit is " << totalLen << endl;
    // 将为填满的buffer写入文件
    buf.write();

    fclose(ftmp);

    return tempPath;
}

string Zip::zip_with_password() {
    /*
        该函数将先对密码进行一次到8位的散列，随后对每个8位进行异或运算
    */
    string tempName = zip_without_password();
    uint8_t hashKey = MyHash::myHash(this->password);
    // cout << "the hash Key is " << (int)hashKey << endl;

    string temptempName = tempName + "temp";

    rename(tempName.c_str(), temptempName.c_str());

    FILE* fsrc = fopen(temptempName.c_str(), "rb");

    FILE* fdst = fopen(tempName.c_str(), "wb");

    uint8_t ch;  // 定义读取文件的空间

    // 先对头码进行混淆，使用相同的初始hashKey
    for (int i = 0; i < 8; i++) {
        fread(&ch, sizeof(uint8_t), 1, fsrc);
        ch ^= hashKey;
        fwrite(&ch, sizeof(uint8_t), 1, fdst);
    }

    while (fread(&ch, sizeof(uint8_t), 1, fsrc)) {
        // uint8_t before = ch;
        ch ^= hashKey;
        // uint8_t oldHash = hashKey;
        hashKey = MyHash::myHashIt(hashKey);
        // cout << "before is " << (int)oldHash << " after is " << (int)hashKey
        //  << endl;
        fwrite(&ch, sizeof(uint8_t), 1, fdst);
    }

    remove(temptempName.c_str());
    fclose(fsrc);
    fclose(fdst);

    // cout << "you are executing an unfinish function!\n";
    return tempName;
}

// uint8_t Zip::myHash(string password) {
//     // 产生一个散列值，如果该值为0，则用默认密码进行生成
//     if (password.size() == 0)
//         password = this->password;
//     uint8_t hash = 0;
//     bool odd = true;
//     for (auto ch : password) {
//         cout << ch << endl;
//         if (odd) {
//             hash += ch;
//         } else {
//             hash *= ch ? ch : 1;
//         }
//         cout << (int)hash << endl;
//         odd = !odd;
//     }
//     if (hash)
//         return hash;
//     return myHash("ILLIYA");
// }

// uint8_t Zip::myHash(uint8_t lastKey) {
//     return (lastKey * lastKey) ^ (lastKey + 1);
// }