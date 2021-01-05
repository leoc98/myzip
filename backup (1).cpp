
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#include<vector>
#include<string>
#include<iostream>

using namespace std;

//filename: ./test/file
struct file_info{//用于保存文件名和对应的inode,用于判断是否为硬链接文件
    char filename[100];
    ino_t inode;
}filesave[1024];
void packagefile(const char* filename,FILE* fpOut){//打包文件

    struct stat stat_buf;
    stat(filename,&stat_buf);
    filesave[1024];
    int i;
    static int n=0;//记录写入文件的真实数量，不包括硬链接文件
    for(i=0;i<=n;i++){//判断文件是否已写入
        if(filesave[i].inode==stat_buf.st_ino){//判断是硬链接文件
            fprintf(fpOut,"h\n%s\n%s\n",filename,filesave[i].filename);//写入标记h->newname->oldname
            return;
        }
    }

    fprintf(fpOut,"f\n%s\n%d\n",filename,(int)stat_buf.st_size);
    FILE *fpIn=fopen(filename,"r");
    char buf[4096];
    while(1){
        int ret=fread(buf,1,sizeof(buf),fpIn);
        if(ret<=0){
            break;
        }
        fwrite(buf,ret,1,fpOut);
    }
    strcpy(filesave[n].filename,filename);//将新打包的文件写入结构体记录
    filesave[n].inode=stat_buf.st_ino;
    n++;
    fclose(fpIn);
}

//dirname: ./test
vector<string> packagedir(const char* dirname,FILE* fpOut){//打包目录
    char filepath[1024];
    vector<string> AllPath;
    fprintf(fpOut,"d\n");//d目录标记
    fprintf(fpOut,"%s\n",dirname);//打包的根目录

    DIR* dir=opendir(dirname);//打开文件目录项
    struct dirent* entry=readdir(dir);
    while(entry){
        //./test/file
        sprintf(filepath,"%s/%s",dirname,entry->d_name);//拼凑每个目录项的路径
        if(entry->d_type==DT_REG){//判断是否为文件
            packagefile(filepath,fpOut);//打包文件
            string temp=filepath;
            AllPath.push_back(temp);
        }
        else if(entry->d_type==DT_DIR){//判断是否为目录,若是就继续递归
            if((strcmp(entry->d_name,".")==0)||
                (strcmp(entry->d_name,"..")==0)){//. ..忽略
                entry=readdir(dir);
                continue;
            }
            vector<string> temp = packagedir(filepath,fpOut);
            for(auto s:temp){
                AllPath.push_back(s);
            }
        }
        entry=readdir(dir);
    }
    /*for(int i=0;i<AllPath.size();i++)
    {
        fprintf(fpOut,"%s\n",AllPath[i]);
    }*/
    closedir(dir);  
    return AllPath;
}

 vector<string> package(const char* dirname,const char *outfile){
    FILE* fpOut=fopen(outfile,"w");
    fprintf(fpOut,"xglpackage\n");//标记打包文件类型
    fprintf(fpOut,"1.0\n");//版本
    vector<string> ret=packagedir(dirname,fpOut);//打包目录
    fclose(fpOut);
    return ret;
}

int unpackagefile(FILE *fin){
    char buf[1024];
    if(fgets(buf,sizeof(buf),fin)==NULL){
        return -1;
    }   
    printf("now upackage type=%s",buf);
    if(strcmp(buf,"d\n")==0){//目录标记
        fgets(buf,sizeof(buf),fin);
        buf[strlen(buf)-1]=0;
        mkdir(buf,0777);
        printf("mkdir %s\n",buf);
    }
    else if(strcmp(buf,"f\n")==0){//文件标记
        fgets(buf,sizeof(buf),fin);
        buf[strlen(buf)-1]=0;
        FILE *out=fopen(buf,"w");
        printf("create file %s\n",buf);
        fgets(buf,sizeof(buf),fin);
        int len=atol(buf);
        printf("filelen %s\n",buf);
        while(len>0){
            int readlen=len<sizeof(buf)?len:sizeof(buf);
            int ret=fread(buf,1,readlen,fin);
            fwrite(buf,1,ret,out);
            len-=ret;
        }
        fclose(out);
    }
    else if(strcmp(buf,"h\n")==0){//硬链接文件标记
        fgets(buf,sizeof(buf),fin);//读取链接文件名
        buf[strlen(buf)-1]=0;
        char oldbuf[1024];//被链接的文件名
        fgets(oldbuf,sizeof(oldbuf),fin);
        oldbuf[strlen(oldbuf)-1]=0;
        link(oldbuf,buf);
    }
    return 0;
}
//解包
int unpackage(const char* packagefile){
    char buf[1024];
    FILE *fin=fopen(packagefile,"r");
    fgets(buf,sizeof(buf),fin);
    if(strcmp(buf,"xglpackage\n")!=0){//判断是否为打包文件类型
        printf("unknown file format\n");
        return -1;
    }
    fgets(buf,sizeof(buf),fin);
    if(strcmp(buf,"1.0\n")==0){//判断版本是否正确
        while(1){
            int ret=unpackagefile(fin);//解包
            if(ret!=0){
                break;
            }
        }
    }else{
        printf("unknown version\n");
        return -1;
    }
    return 0;

}
//./package -c test packagetest
//./package -u packagetest
int main(int argc,char *argv[])
{
    if(argc==1){
        printf("usage:\t./-c file1 file2  or ./-u file1\n");
        return -1;
    }

    //const char *option=argv[1];
    if(strcmp(argv[1],"-c")==0){
        const char* dirname=argv[2];
        const char* outfile=argv[3];
        package(dirname,outfile);    //打包
        return 0;
    }else if(strcmp(argv[1],"-u")==0){
        const char* packagefile=argv[2];    
        return unpackage(packagefile);          //解包 
    }

    printf("argv[1] error\n");
    return -1;
}