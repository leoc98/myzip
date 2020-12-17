#ifndef BUFFER
#define BUFFER
#include <inttypes.h>
#include <memory.h>
#include <stdio.h>
#define BUFFER_SIZE 2
#define OVERFLOW_MAX 32
#define bitsMask(x) (1 << (x)) - 1
#define bitMast(x) 1 << (x)
class Buffer {
   private:
    void init();
    uint8_t buf[BUFFER_SIZE];
    uint8_t sp8;
    uint16_t sp16;
    FILE* fp;
    int now;
    int sp8filled;

   public:
    int fill(uint8_t* data, int bitLength);
    int fill_8(uint8_t* data, int st, int bitLength);
    void putsp8();
    Buffer(FILE* fp) : fp(fp) { init(); }
    uint8_t* getBuffer() { return buf; }
    void reset() { init(); }
    int write();
};

#endif