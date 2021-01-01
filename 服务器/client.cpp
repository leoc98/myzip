#include "filetrans.h"

void sendPackup(string sendURL,string storeURL){
    Sender sd("127.0.0.1",6666);
    Receiver rec("",6667);
    sd.send_msg(SEND_YOU_PACKUP);
    rec.mylisten();
    sleep(2);
    sd.send_packup(sendURL,storeURL);
}

void recPackup(string restoreName,string whereToPlace){
    Sender sd("127.0.0.1",6666);
    Receiver rec("",6667);
    sd.send_msg(SEND_ME_PACKUP);
    rec.mylisten();
    sleep(2);
    sd.send_msg(restoreName);
    rec.rec_packup(whereToPlace);
}


int main(){
    sendPackup("../测试文件/1.jpg","apackup");
    sleep(3);
    recPackup("apackup","arestore.jpg");
    return 0;
}