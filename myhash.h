#ifndef __MYHASH__

#define __MYHASH__
#define RANDOM_HEAD "ILLIYAMY"
#define RANDOM_HEAD_SIZE 64
#include <inttypes.h>
#include <string>

class MyHash {
   public:
    static uint8_t myHash(string password) {
        // 用于首次生成哈希
        // 产生一个散列值，如果该值为0，则用默认密码进行生成
        uint8_t hash = 0;
        bool odd = true;
        for (auto ch : password) {
            cout << ch << endl;
            if (odd) {
                hash += ch;
            } else {
                hash *= ch ? ch : 1;
            }
            cout << (int)hash << endl;
            odd = !odd;
        }
        if (hash)
            return hash;
        return myHash("ILLIYA");
    }

    static uint8_t myHashIt(uint8_t lastKey) {
        // 用于迭代生成hash
        return (lastKey * lastKey) ^ (lastKey + 1);
    }
};

// uint8_t MyHash::myHash(string password) {
//     // 产生一个散列值，如果该值为0，则用默认密码进行生成
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

// uint8_t MyHash::myHashIt(uint8_t lastKey) {
//     // 用于迭代生成hash
//     return (lastKey * lastKey) ^ (lastKey + 1);
// }

#endif