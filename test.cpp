// #include "halfmanTree.h"
#include <iostream>
// #define __TEST_FILENAME_SUFFIX__
#include "zipHandler.h"
#ifndef __TEST_FILENAME_SUFFIX__
#define __TEST_FILENAME_SUFFIX__
#endif

bool test_treenode() {
    TreeNode a(1, 100);
    TreeNode b(2, 200);
    TreeNode* c = a + b;
    TreeNode d(2, 100);
    printf("%s",a.print());
    printf("%s",b.print());
    cout << a.print() << endl;
    cout << b.print() << endl;
    cout << c->print() << endl;
    cout << "c left child msg:"<<endl;
    cout << c->getChild(LEFT)->print()<<endl;
    cout << "c right child msg:"<<endl;
    cout << c->getChild(RIGHT)->print()<<endl;
    cout << "a<b?" << (a<b) <<endl;
    cout << "a=b?" << (a==b) <<endl;
    cout << "a>b?" << (a>b) <<endl;
    cout << "a=d?" << (a==d) <<endl;
    return true;
}

bool test_halfman_tree() {
    map<uint8_t, unsigned int> cntMap;
    cntMap[2] = 100;
    cntMap[1] = 200;
    cntMap[5] = 300;
    cntMap[4] = 250;
    cntMap[3] = 124;
    HalfmanTree ht(cntMap);
    ht.print();
    // ht.halfDirectory;
    cout << "the size of halfDirectory:" << ht.halfDirectory.size() << endl;
    for (auto key : ht.halfDirectory) {
        ht.print();
    }
    cout << "the total size of this tree is " << ht.totalSize() << endl;
    return true;
}

bool getFile() {
    FILE* f = fopen("test.txt.ILLIYA", "rb");
    uint8_t ch;
    while (fread(&ch, sizeof(uint8_t), 1, f)) {
        cout << (int)ch << endl;
    }
    return true;
}

bool test_zip(string path, string password) {
    // Zip z("测试用涩情图片.jpg", "");
    // Zip z("test.txt", "");
    if (password.size() == 1)
        password.clear();
    Zip z(path, password);
    z.zip();
    // cout<<"the origin path is "
    return true;
}

bool test_unzip(string path, string password) {
    // UnZip uz("测试用涩情图片.jpg.ILLIYA", "");
    // UnZip uz("test.txt.ILLIYA", "");
    if (password.size() == 1)
        password.clear();
    UnZip uz(path + ".ILLIYA", password);
    string unzipFileName = uz.unzip();
    if (unzipFileName.size() == 0) {
        cout << "解压失败" << endl;
    } else {
        cout << "解压完成：新文件名为 " << unzipFileName << endl;
    }
    return true;
    // return true;
    // cout<<"the origin path is "
    // return true;
}

bool test_high_low() {
    FILE* f = fopen("test.txt.ILLIYA", "rb");
    char ch[4 * 8];
    fread(&ch, sizeof(uint8_t), 4 * 8, f);
    char a;
    fread(&a, sizeof(char), 1, f);
    fclose(f);
    return true;
}

bool test_rev_tree() {
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
        cout<<"the key is "<<key.first<<" and the value is "<<
        key.second<<endl;
    }
    // cout<<"the total size of this tree is "<<ht.totalSize()<<endl;
    return true;
}

bool test_del_haltree() {
    test_halfman_tree();
    cout << "I am not crashed, oh yeah!" << endl;
    return true;
}

bool test_zip_then_unzip() {
    string path = "./测试文件/1.jpg";
    string password = "123";

    Zip z(path, password);
    z.zip();

    UnZip uz(path + ".ILLIYA", "12");
    string retName = uz.unzip();
    cout << "return name is" << retName.size() << endl;

    cout << "successfully running all program!" << endl;
    return true;
}

int main() {
    test_treenode();
    // test_halfman_tree();
    test_rev_tree();
    test_del_haltree();
    test_zip_then_unzip();


    // test_del_haltree();
    // int ctl;
    // cout << "请输入控制编号，1为解压，0为压缩" << endl;
    // string path, password;
    // cin >> ctl;
    // cout << "请输入路径" << endl;
    // cin >> path;
    // cout << "请输入密码" << endl;
    // cin >> password;
    // if (ctl) {
    //     cout << "进行了解压缩" << endl;
    //     test_unzip(path, password);
    // } else {
    //     cout << "进行了压缩" << endl;
    //     test_zip(path, password);
    // }

    // FILE* fsrc = fopen("测试用涩情图片.jpg", "rb");
    // FILE* fdst = fopen("测试用涩情图片移植版.jpg", "wb");

    // uint8_t ch;

    // while (fread(&ch, sizeof(uint8_t), 1, fsrc)) {
    //     fwrite(&ch, sizeof(uint8_t), 1, fdst);
    // }

    // fclose(fsrc);
    // fclose(fdst);

    // test_treenode();
    // test_halfman_tree();
    // test_unzip();
    // getFile();
    // test_zip();
    // test_high_low();
    // test_rev_tree();
    return 0;
}
