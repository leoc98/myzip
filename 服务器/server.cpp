#include "filetrans.h"
#define PACKUP_NAME "PACKUP.ILLIYA"


int runServer(){
    int port = 6666;
    Receiver rec("",port);
    Sender sd("127.0.0.1",6667);
    while(true){
        string msg = rec.mylisten();
        sleep(2);
        if(strcmp(msg.c_str(),SEND_YOU_PACKUP)==0){
            // sd.send_msg("OK");
            // sleep(2);
            printf("test1\n");
            sd.send_msg("OK");
            string filename=rec.mylisten();
            rec.rec_packup(filename);
            printf("finish\n");
        }else if(strcmp(msg.c_str(),SEND_ME_PACKUP)==0){
            sd.send_msg("OK");
            printf("test2\n");
            string filename=rec.mylisten();
            sleep(2);
            sd.send_packup(filename,"");
            printf("finish\n");
        }else{
            printf("err header!\n");
        }
    }
    
    // printf("receive msg: %s",msg.c_str());
    // rec.rec_packup("测试1.jpg");
    return 0;
}

int main(){
    runServer();
    return 0;
}