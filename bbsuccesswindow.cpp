#include "bbsuccesswindow.h"
#include "ui_bbsuccesswindow.h"
#include "widget.h"
#include "ybsuccesswindow.h"
#include "filetrans.h"
#include "compare.h"
extern void sendPackup(string,string);
bbsuccessWindow::bbsuccessWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bbsuccessWindow)
{
    ui->setupUi(this);
}

bbsuccessWindow::~bbsuccessWindow()
{
    delete ui;
}

void bbsuccessWindow::on_pushButton_2_clicked()
{
    printf("%s\n",tempName.c_str());
    sendPackup("/home/huangtt/test.szk.ILLIYA","test.szk.ILLIYA");
    ybsuccessWindow *win = new ybsuccessWindow;
    win->show();
    delete this;
}

void bbsuccessWindow::on_pushButton_clicked()
{
    Widget *win = new Widget;
    win->show();
    delete this;
}


