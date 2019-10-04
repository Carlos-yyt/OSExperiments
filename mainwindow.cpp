﻿#include "mainwindow.h"
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

    srand(time(0));//随机数的种子

    ui->exTimesLcdNum->setStyleSheet("border: 1px solid green; color: green; background: silver;");
    ui->exTimesLcdNum->setDigitCount(8);
    ui->doManyExLineEdit->setValidator(new QIntValidator(1,100000,this));
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
    QFile::remove(pathTXTStr);

//    //===DeleteFile===总有问题
//    int fileStatusValue;

//    fileStatusValue=DeleteFile(pathTXTWChar);//TODO 为什么没有文件也能删除成功？找找
//    //qDebug()<<"fileStatusValue："+QString::number(fileStatusValue);
//    //qDebug()<<"ERROR_FILE_NOT_FOUND:"+QString::number(ERROR_FILE_NOT_FOUND);
//    if(fileStatusValue==ERROR_FILE_NOT_FOUND){
//        ui->delFileLab->setText("没有找到文件。可以直接进行程序。");
//    }else if(fileStatusValue==ERROR_ACCESS_DENIED){
//        ui->delFileLab->setText("有文件，但是被设置成了只读文件。");
//    }else {
//        ui->delFileLab->setText("已经找到文件并删除。现在没有文件了。");
//    }
    exTimes=0;
    exDelay=0;
    ui->delayLab->setText(QString::number(exDelay*1.0/(exTimes*10),'g',6));
    ui->exTimesLcdNum->display(exTimes);//改变记分板上的试验次数
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
        WriteFile(pFile, FileHead, strlen(FileHead), NULL, NULL);//写入TXT文件
    }
}

//获取rdtsc
inline cycles_time MainWindow::currentcycles(){
    cycles_time result;
        __asm__ __volatile__ ("rdtsc" : "=A" (result));//可惜不能直接返回它的返回值，要走一个中间变量。
        return result;
}

//连续获取10次rdtsc
inline void MainWindow::get10NS(){
    Sleep(0);//释放当前线程所剩余的时间片（如果有剩余的话），尽量让下面20条语句享有完整的时间片

    /*
     * 1 - 不设置一个不断自增的变量是为了节省运行时的时间。
     * 2 - 每句写2遍是因为取一次需要10ns，而题目刚好要求20ns。
     */
    __asm__ __volatile__ ("rdtsc" : "=A" (ns[0]));
    __asm__ __volatile__ ("rdtsc" : "=A" (ns[0]));

    __asm__ __volatile__ ("rdtsc" : "=A" (ns[1]));
    __asm__ __volatile__ ("rdtsc" : "=A" (ns[1]));

    __asm__ __volatile__ ("rdtsc" : "=A" (ns[2]));
    __asm__ __volatile__ ("rdtsc" : "=A" (ns[2]));

    __asm__ __volatile__ ("rdtsc" : "=A" (ns[3]));
    __asm__ __volatile__ ("rdtsc" : "=A" (ns[3]));

    __asm__ __volatile__ ("rdtsc" : "=A" (ns[4]));
    __asm__ __volatile__ ("rdtsc" : "=A" (ns[4]));

    __asm__ __volatile__ ("rdtsc" : "=A" (ns[5]));
    __asm__ __volatile__ ("rdtsc" : "=A" (ns[5]));

    __asm__ __volatile__ ("rdtsc" : "=A" (ns[6]));
    __asm__ __volatile__ ("rdtsc" : "=A" (ns[6]));

    __asm__ __volatile__ ("rdtsc" : "=A" (ns[7]));
    __asm__ __volatile__ ("rdtsc" : "=A" (ns[7]));

    __asm__ __volatile__ ("rdtsc" : "=A" (ns[8]));
    __asm__ __volatile__ ("rdtsc" : "=A" (ns[8]));

    __asm__ __volatile__ ("rdtsc" : "=A" (ns[9]));
    __asm__ __volatile__ ("rdtsc" : "=A" (ns[9]));
}

//获得本机主频
inline machineFrequency MainWindow::getThisMachineFrequency(){
    LARGE_INTEGER fre;
    QueryPerformanceFrequency(&fre);
    return (machineFrequency)fre.QuadPart;
}

void MainWindow::on_writeTXTBtn_clicked(bool checked)
{
    //===设置变量
    char write_content[100] = { 0 }; //写入内容的数组
    QTime current_time;//用于获得时分秒
    int rand_num[10];//随机数

    //===生成随机数
    for (int i=0;i<10;i++) {
        rand_num[i]=rand()%50+1;
    }

    //===获取主频
    mFre=getThisMachineFrequency();

    //===获取10个rdtsc
    get10NS();

    //===实验组号，方便多次实验时观察记录
    strcat(write_content,"第");
    strcat(write_content,QString::number(++exTimes).toStdString().data());
    strcat(write_content,"次实验（插入10条记录）");
    strcat(write_content, "\r\n"); //换行
    WriteFile(pFile, write_content, strlen(write_content), NULL, NULL);

    //===写记录，循环写入10条
    for(int id=0;id<10;id++){
        memset(write_content,0,100);//清空write_content数组

        //===接上id===
        strcat(write_content,QString::number(id).toStdString().data());//写入记录的编号

        //===接上name===
        strcat(write_content, "\t\t"); //接上间隔
        strcat(write_content, "像素"); //接上名字

        //===接上input===
        strcat(write_content, "\t\t"); //接上间隔
        strcat(write_content, QTime::currentTime().toString("hh:mm:ss").toStdString().data());//接上当前的时分秒

        //===接上ns===
        strcat(write_content, "\t\t"); //接上间隔
        strcat(write_content,QString::number((DWORD)(((double(ns[id]))/mFre)*1000000)).toStdString().data()); //接上ns
        //===接上随机数===
        strcat(write_content, "\t"); //接上间隔
        strcat(write_content, QString::number(rand_num[id]).toStdString().data()); //写入随机数
        strcat(write_content, "\r\n"); //换行

        //===写文件===
        WriteFile(pFile, write_content, strlen(write_content), NULL, NULL);//向TXT文件写入
    }

    //将本组间隔时间加入累计
    for(int i=0;i<9;i++){
        exDelay+=(int)(((double(ns[i+1]-ns[i]))/mFre)*1000000);
    }

    ui->delayLab->setText(QString::number(exDelay*1.0/(exTimes*9),'g',6));//更新平均间隔时间。不是exTimes*10！我真是蠢死了。
    ui->exTimesLcdNum->display(exTimes);//更新记分板上的试验次数。
}

void MainWindow::on_doManyExBtn_clicked()
{
    int exTimes=0;//接收用户指定的实验次数
    exTimes=ui->doManyExLineEdit->text().toInt();
    for(int i=0;i<exTimes;i++){
        on_writeTXTBtn_clicked(true);//一次实验
    }
}

void MainWindow::on_doManyExBtn_clicked(bool checked)
{

}
