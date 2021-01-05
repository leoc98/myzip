#include "bbbwindow.h"
#include "ui_bbbwindow.h"
#include "widget.h"
#include "ybsuccesswindow.h"
#include "bbsuccesswindow.h"
#include "failwindow.h"
#include "zipHandler.h"
#include "backup.h"
#include "filetrans.h"


//#include "QString"


bbbWindow::bbbWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bbbWindow)
{
    ui->setupUi(this);
}

bbbWindow::~bbbWindow()
{
    delete ui;
}

void bbbWindow::on_pushButton_5_clicked()
{
    QString pwd =ui->lineEdit->text();
    QString pwd2=ui->lineEdit_2->text();
    if(pwd==pwd2)
    {  
        QString path =ui->lineEdit_3->text();
        string dirname = (const char*)path.toLocal8Bit();
        string outfile = dirname+".szk";
        vector<string> oldtree=package(dirname.c_str(),outfile.c_str());
        // printf("the size of tree is %d\n",oldtree.size());
        // for(auto s:oldtree){
        //     printf("%s\n",s.c_str());
        // }
        Zip zip(outfile, (const char*)pwd.toLocal8Bit());
        zip.zip();
        outfile+=".ILLIYA";
        tempName = outfile;
        string newOutfile=outfile+"new";
        
        vector<string> newtree=package(dirname.c_str(),outfile.c_str());
        string printout=getDiff(oldtree,newtree); // 如果需要验证，将该字符串输出，注意可能要进行string到Qstring的转换

        remove(newOutfile);
        remove(outfile);
        bbsuccessWindow *win = new bbsuccessWindow;
        win->show();
         delete this;
    }
    else
    {
        failWindow *win = new failWindow;
        win->show();
        delete this;
    }

}

void bbbWindow::on_pushButton_clicked()
{
    Widget *win = new Widget;
    win->show();
    delete this;
}

string getDiff(vector<string> oldTree, vector<string> newTree) {
    struct Comp_tree ct = compare(oldTree,newTree);
    string re;
    if(ct.a.size()!=0){
        re+="多了\n";
        for(auto s:ct.a){
            re+=s;
            re+='\n';
        }
    }
    if(ct.b.size()!=0){
        re+="少了\n";
        for(auto s:ct.b){
            re+=s;
            re+='\n';
        }
    }
    return re;
}
