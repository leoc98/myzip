// #include "halfmanTree.h"
#include <iostream>
#include "zipHandler.h"

// bool test_treenode()
// {
//     TreeNode a(1, 100);
//     TreeNode b(2, 200);
//     TreeNode *c = a + b;
//     TreeNode d(2, 100);
//     // printf("%s",a.print());
//     // printf("%s",b.print());
//     // cout << a.print() << endl;
//     // cout << b.print() << endl;
//     // cout << c->print() << endl;
//     // cout << "c left child msg:"<<endl;
//     // cout << c->getChild(LEFT)->print()<<endl;
//     // cout << "c right child msg:"<<endl;
//     // cout << c->getChild(RIGHT)->print()<<endl;
//     // cout << "a<b?" << (a<b) <<endl;
//     // cout << "a=b?" << (a==b) <<endl;
//     // cout << "a>b?" << (a>b) <<endl;
//     // cout << "a=d?" << (a==d) <<endl;
//     return true;
// }

// bool test_halfman_tree()
// {
//     map<uint8_t, unsigned int> cntMap;
//     cntMap[2] = 100;
//     cntMap[1] = 200;
//     cntMap[5] = 300;
//     cntMap[4] = 250;
//     cntMap[3] = 124;
//     HalfmanTree ht(cntMap);
//     ht.print();
//     // ht.halfDirectory.
//     cout << "the size of halfDirectory:" << ht.halfDirectory.size() << endl;
//     for (auto key : ht.halfDirectory)
//     {
//         ht.halfDirectory[key.first]->print();
//     }
//     cout<<"the total size of this tree is "<<ht.totalSize()<<endl;
//     return true;
// }

bool test_zip() {
    Zip z("test.txt","");
    z.zip();
    // cout<<"the origin path is "
    return true;
}

bool getFile(){
    FILE* f = fopen("test.txt.ILLIYA","r");
    uint8_t ch;
    while(fread(&ch,sizeof(uint8_t),1,f)){
        cout<<(int)ch<<endl;
    }
    return true;
}

bool test_unzip() {
    UnZip uz("test.txt.ILLIYAtemp","asdf");
    return uz.passwordCheck();
    // return true;
    // cout<<"the origin path is "
    // return true;
}

bool test_high_low(){
    FILE* f = fopen("test.txt.ILLIYA","r");
    char ch[4*8];
    fread(&ch,sizeof(uint8_t),4*8,f);
    char a;
    fread(&a,sizeof(char),1,f);
    fclose(f);
    return true;
}

bool test_rev_tree(){
    map<uint8_t, unsigned int> cntMap;
    cntMap['a'] = 20;
    cntMap['b'] = 2;
    cntMap['c'] = 1;
    // cntMap[4] = 250;
    // cntMap[3] = 124;
    HalfmanTree ht(cntMap);
    // ht.print();
    // ht.halfDirectory.
    // cout << "the size of halfDirectory:" << ht.halfDirectory.size() << endl;
    ht.formReverseDir();
    for (auto key : ht.revHalfDirectory)
    {
        cout<<"the key is "<<key.first<<" and the value is "<< key.second<<endl;
    }
    // cout<<"the total size of this tree is "<<ht.totalSize()<<endl;
    return true;
}

int main() {
    // test_treenode();
    // test_halfman_tree();
    // cout<<test_unzip()<<endl;
    // getFile();
    // test_zip();
    // test_high_low();
    test_rev_tree();
    return 0;
}