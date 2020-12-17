#ifndef __HALFMANTREE__

#define __HALFMANTREE__
#include <inttypes.h>
#include <memory.h>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
#define ELEM_LENGTH 8
#define ELEM_LENGTH_SHIFT 3
typedef void* (*HandleFunctionAfter)(void* thisNode,
                                     void* thisRet,
                                     void* leftRet,
                                     void* middleRet,
                                     void* rightRight,
                                     void* arg);
typedef void* (*HandleFunctionMiddle)(void* thisNode,
                                      void* thisRet,
                                      void* leftRet,
                                      void* arg);
typedef void* (*HandleFunctionBefore)(void* thisNode, void* arg);

using namespace std;

/*
    该哈夫曼树由外部传入一个计数表，生成哈夫曼树后进行dfs，生成映射表供外部调用
*/

enum Child {
    RIGHT = 0,
    LEFT = 1,
};

class TreeNode {
   private:
    uint8_t key;
    TreeNode* leftChild;
    TreeNode* rightChild;
    void init(){};

   public:
    uint32_t cnt;
    TreeNode(uint8_t key,
             uint32_t cnt,
             TreeNode* leftChild = NULL,
             TreeNode* rightChild = NULL)
        : key(key), cnt(cnt), leftChild(leftChild), rightChild(rightChild) {
        init();
    }

    bool operator==(const TreeNode ptn) const { return cnt == ptn.cnt; }

    bool operator<(const TreeNode ptn) const { return cnt > ptn.cnt; }

    bool operator>(const TreeNode ptn) const { return cnt > ptn.cnt; }

    TreeNode* operator+(TreeNode& ptn) {
        TreeNode* newNode = new TreeNode(0, cnt + ptn.cnt, this, &ptn);
        return newNode;
    }

    bool isLeaf() { return (leftChild == NULL) | (rightChild == NULL); }

    TreeNode* getChild(Child whichChild) {
        if (whichChild == LEFT)
            return leftChild;
        return rightChild;
    }

    void setChild(Child whichChild, TreeNode* child) {
        if (whichChild == LEFT) {
            this->leftChild = child;
        } else {
            this->rightChild = child;
        }
    }

    uint8_t getKey() { return key; }

    string print() {
        char cp[50];
        sprintf(cp, "this Tree key is: %d\ncnt is %d\n\n", (int)key, (int)cnt);
        string re = cp;
        return re;
    }
};

// class TreeNodeShadow: Public TreeNode{
// public:

// };

class HalfmanValue {
   private:
    void init() {

        // 不需要再进行逆转，因为写入的数据用byte读出后从高位读出可以按顺序获得编码
        // uint8_t* revVal = new uint8_t[bitLength >> ELEM_LENGTH_SHIFT];
        // uint8_t* low;
        // for (int i = 0; i < bitLength; i++) {
        //     low = &value[i / 8];
        //     if (*low & 1) {
        //         int revInd = (bitLength - i - 1) / 8;
        //         int offset = (bitLength - i - 1) % 8;
        //         revVal[revInd] |= 1 << offset;
        //     }
        //     (*low) >>= 1;
        // }
        // uint8_t* temp;
        // temp = value;
        // value = revVal;
        // delete temp;
    }

   public:
    uint8_t* value;
    int bitLength;
    // uint8_t mask;
    HalfmanValue(){};
    HalfmanValue(uint8_t* value, int bitLength) : bitLength(bitLength) {
        //为value分配空间
        int size = bitLength >> ELEM_LENGTH_SHIFT;
        size++;
        this->value = new uint8_t[size];
        memcpy(this->value, value, size);
        init();
    }
    ~HalfmanValue() { delete value; }
    void print() {
        int bitmask = ~(-1 << bitLength);
        int* st = (int*)value;
        int tv = (*st) & bitmask;
        cout << "this value is:" << tv << " bitlength :" << bitLength << endl;
    }
};

class HalfmanTree {
   private:
    TreeNode* root;
    void init();
    static void* codingAfter(void* thisNode,
                             void* thisRet,
                             void* leftRet,
                             void* middleRet,
                             void* rightRet,
                             void* arg);
    static void* codingMiddle(void* thisNode,
                              void* thisRet,
                              void* leftRet,
                              void* arg);
    static void* codingBefore(void* thisNode, void* arg);
    map<uint8_t, HalfmanValue*> halfDirectory;
    map<string, uint8_t> revHalfDirectory;

   public:
    map<uint8_t, uint32_t> cntMap;

    HalfmanTree(map<uint8_t, uint32_t> cntMap) : cntMap(cntMap) {
        halfDirectory.clear();
        init();
    }
    void* dfs(TreeNode* root,
              HandleFunctionBefore before,
              HandleFunctionMiddle middle,
              HandleFunctionAfter after,
              void* arg);
    HalfmanValue* translate(uint8_t key);
    uint8_t revTranslate(string key,bool* exist);

    void formReverseDir();

    int totalSize();
    int getDirectorySize() { return halfDirectory.size(); }

    static void* printNode(void* thisNode, void* arg) {
        TreeNode* tn = (TreeNode*)thisNode;
        if (tn)
            cout << tn->print() << endl;
        return NULL;
    }
    void print() { dfs(this->root, printNode, nopmiddle, nopafter, NULL); }
    void print_key() {
        for (auto key : halfDirectory) {
            halfDirectory[key.first]->print();
        }
    }
    static void* nopbefore(void* thisNode, void* arg) { return NULL; }
    static void* nopmiddle(void* thisNode,
                           void* thisRet,
                           void* leftRet,
                           void* arg) {
        return NULL;
    }
    static void* nopafter(void* thisNode,
                          void* thisRet,
                          void* leftRet,
                          void* middleRet,
                          void* rightRet,
                          void* arg) {
        return NULL;
    }
};

#endif