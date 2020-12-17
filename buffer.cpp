#include "buffer.h"

void Buffer::init() {
    // memset(tempbuf, 0, OVERFLOW_MAX);
    memset(buf, 0, BUFFER_SIZE);
    now = 0, sp8filled = 0;
    sp8 = 0, sp16 = 0;
    // fp = fopen()
}
// st为需要装填入的起始坐标
// 返回值为状态，0为成功
int Buffer::fill_8(uint8_t* data, int st, int bitLength) {
    if (bitLength == 0)
        return 0;
    uint8_t* psp16low = (uint8_t*)&sp16;
    uint8_t* psp16high = psp16low;
    psp16high++;
    uint8_t bitM = bitsMask(bitLength);
    *psp16low = data[st];
    *psp16low &= bitM;

    *psp16high = 0;

    sp16 = sp16 << sp8filled;
    sp8 |= *psp16low;

    if (bitLength + sp8filled > 8) {
        // 有比特溢出，则将sp8写入缓冲区，并读取溢出位进入sp8，同时更新sp8filled
        putsp8();
        sp8 = *psp16high;
        sp8filled = bitLength + sp8filled - 8;
    } else {
        // 无比特溢出，则将sp8filled更新即可
        sp8filled += bitLength;
    }
    return 0;
}

void Buffer::putsp8() {
    buf[now++] = sp8;
    if (now == BUFFER_SIZE) {
        write();
        uint16_t temp16 = sp16;
        int tempsp8filled = sp8filled;
        reset();
        sp16 = temp16;
        sp8filled = tempsp8filled;
    }
}

// 如果缓冲满或者溢出，则会自动写入fp。
// 返回操作状态，0为成功
int Buffer::fill(uint8_t* data, int bitLength) {
    int st = 0;
    int last = bitLength % 8;
    while (st * 8 + 8 <= bitLength) {
        fill_8(data, st++, 8);
    }

    fill_8(data, st, last);

    return 0;
}

int Buffer::write() {
    if (now == BUFFER_SIZE) {
        return fwrite(buf, now, 1, fp);
    }
    putsp8();
    return fwrite(buf, now, 1, fp);
}
