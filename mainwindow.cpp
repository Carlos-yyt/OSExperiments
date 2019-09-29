#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DWORD size=GetModuleFileNameA(NULL,path,MAX_PATH);//MAX_PATH在minwindef.h的第33行有定义：#define MAX_PATH 260
    pathStr=path;
    if (size != 0){
        qDebug()<<"程序当前目录："+pathStr;//输出当前目录信息
        int lenPath=strlen(path);
        while(path[lenPath]!='\\'){
            path[lenPath]='\0';//给地址结尾添上字符串结束标志
        }
        strcpy(pathTXT,path);//文件所在地址
        strcat(pathTXT,TXT_FILE_NAME);//加上文件名
        pathTXTStr=pathTXT;
        qDebug()<<"文本文件地址："+pathTXTStr;//输出当前目录信息
    }else{
        qDebug()<<"获取文件地址失败";
        exit(0); //若获取失败，则直接退出程序
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::charToWChar(char* src,WCHAR *dest){
    memset(dest,0,sizeof(dest));

    MultiByteToWideChar(CP_ACP,0,src,strlen(src)+1,dest,sizeof(dest)/sizeof(dest[0]));//MultiByteToWideChar函数是将多字节转换为宽字节的一个API函数 char* 转 LPCWSTR

    return true;
}

//删除现有文件
void MainWindow::on_delExistBtn_clicked(bool checked)
{
    charToWChar(pathTXT,pathTXTWChar);
    int fileStatusValue=DeleteFile(pathTXTWChar);
}
