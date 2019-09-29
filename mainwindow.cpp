#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DWORD size=GetModuleFileNameA(nullptr,path,MAX_PATH);//MAX_PATH在minwindef.h的第33行有定义：#define MAX_PATH 260
    pathStr=path;
    if (size != 0)
        qDebug()<<"程序当前目录："+pathStr;//输出当前目录信息
    else
        exit(0); //若获取失败，则直接退出程序
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_delExistBtn_clicked(bool checked)
{
    WCHAR pathWChar[256];
    memset(pathWChar,0,sizeof(pathWChar));

    //MultiByteToWideChar函数是将多字节转换为宽字节的一个API函数 char* 转 LPCWSTR
    MultiByteToWideChar(CP_ACP,0,path,strlen(path)+1,pathWChar,sizeof(pathWChar)/sizeof(pathWChar[0]));

    int fileStatusValue=DeleteFile(pathWChar);
    //下面继续写文件，在那之前先获取TXT地址
}
