#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DWORD size=GetModuleFileNameA(nullptr,path,MAX_PATH);//MAX_PATH在minwindef.h的第33行有定义：#define MAX_PATH 260
    pathStr=path;
    if (size != 0){
        qDebug()<<"程序当前目录："+pathStr;//输出当前目录信息
        int lenPath=strlen(path);
        while(path[lenPath]!='\\'){
            path[lenPath--]='\0';//给地址结尾添上字符串结束标志
        }
        strcpy(pathTXT,path);//文件所在地址
        strcat(pathTXT,TXT_FILE_NAME);//加上文件名
        pathTXTStr=pathTXT;
        qDebug()<<"文本文件地址："+pathTXTStr;//输出当前目录信息
        //charToWChar(pathTXT,pathTXTWChar);//转好字符备用
    }else{
        qDebug()<<"获取文件地址失败";
        exit(0); //若获取失败，则直接退出程序
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

//字符转换
bool MainWindow::charToWChar(char* src,WCHAR *dest){
    memset(dest,0,sizeof(dest));

    MultiByteToWideChar(CP_ACP,0,src,strlen(src)+1,dest,sizeof(dest)/sizeof(dest[0]));//MultiByteToWideChar函数是将多字节转换为宽字节的一个API函数 char* 转 LPCWSTR

    return true;
}

//删除现有文件
void MainWindow::on_delExistBtn_clicked(bool checked)
{
    int fileStatusValue;
    fileStatusValue=DeleteFile(pathTXTWChar);//TODO 为什么没有文件也能删除成功？找找
    //qDebug()<<"fileStatusValue："+QString::number(fileStatusValue);
    //qDebug()<<"ERROR_FILE_NOT_FOUND:"+QString::number(ERROR_FILE_NOT_FOUND);
    if(fileStatusValue==ERROR_FILE_NOT_FOUND){
        ui->delFileLab->setText("没有找到文件。可以直接进行程序。");
    }else if(fileStatusValue==ERROR_ACCESS_DENIED){
        ui->delFileLab->setText("有文件，但是被设置成了只读文件。");
    }else {
        ui->delFileLab->setText("已经找到文件并删除。现在没有文件了。");
    }
}

//创建TXT文件
void MainWindow::on_createFileBtn_clicked(bool checked)
{
    pFile=CreateFile(
                pathTXTStr.toStdWString().c_str(),//文件地址
                GENERIC_READ|GENERIC_WRITE,//文件可读可写
                FILE_SHARE_READ,//保护期间，可读不可写
                NULL,//设为 NULL，子进程不能继承本句柄
                CREATE_ALWAYS,//创建文件，会改写前一个文件。虽然之前delete过，但出于鲁棒性还是这么写吧。
                FILE_ATTRIBUTE_NORMAL,//文件属性设置为默认
                NULL//不设置文件句柄
                );
    if (pFile == INVALID_HANDLE_VALUE)
    {
        ui->createFileLab->setText("失败！文件未创建。");
        CloseHandle(pFile);
    }else {
        ui->createFileLab->setText("成功！文件已创建。");
        char FileHead[] = "id\t\tname\t\tinputTime\t\tns\t\truntimes\r\n";//写文件头
        WriteFile(pFile, FileHead, strlen(FileHead), NULL, NULL);
    }
}

inline cycles_time MainWindow::currentcycles(){
    cycles_time result;
        __asm__ __volatile__ ("rdtsc" : "=A" (result));//可惜不能直接返回它的返回值，要走一个中间变量。
        return result;
}

void MainWindow::on_writeTXTBtn_clicked(bool checked)
{
    char write_content[100] = { 0 }; //写入内容的数组

    QTime current_time;

    cycles_time curTime;//用于存放时间戳

    for(int id=0;id<10;id++){
        memset(write_content,0,100);
        strcat(write_content,QString::number(id).toStdString().data()); //写入id数
        strcat(write_content, "\t\t"); //写入间隔
        strcat(write_content, "像素\t\t"); //写入名字

        //插入时间
        strcat(write_content, QTime::currentTime().toString("hh:mm:ss").toStdString().data());
        strcat(write_content, "\t\t"); //写入间隔

        curTime=currentcycles();//获取当前时间戳
        strcat(write_content, QString::number(curTime).toStdString().data()); //写入时间戳
        strcat(write_content, "\t\t\r\n"); //写入间隔
        //DWORD SizeToWrite = sizeof(write_content);
        WriteFile(pFile, write_content, strlen(write_content), NULL, NULL);
    }

}
