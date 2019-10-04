#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "windows.h"
#include <qdebug.h>
#include <QString>
#include <QDateTime>
#include <QTime>
#include <random>
#include <QFile>

#define TXT_FILE_NAME "19317119_win32.txt"

using std::default_random_engine;
using std::uniform_int_distribution;

typedef unsigned long long cycles_time;//用来存放64位的时间戳计数器里面的内容
typedef unsigned long long machineFrequency;//用来存放机器主频
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);//生成EXE文件和TXT文件的地址的各种编码
    ~MainWindow();

private slots:

    void on_delExistBtn_clicked(bool checked);//检查并删除现有文件

    bool charToWChar(char* src,WCHAR *dest);//char字符串向WChar转换

    void on_createFileBtn_clicked(bool checked);//创建一个TXT文件并写入文件表头

    void on_writeTXTBtn_clicked(bool checked);//向TXT文件写入一组（10条）记录

    void on_doManyExBtn_clicked();//进行多次实验

    void on_doManyExBtn_clicked(bool checked);

private:
    char path[MAX_PATH];//当前程序地址
    QString pathStr;//当前程序地址--QString形式

    char pathTXT[MAX_PATH];//TXT文件的位置
    QString pathTXTStr;//当前TXT文件地址--QString形式
    WCHAR pathTXTWChar[MAX_PATH];//当前TXT文件地址--WCHAR形式

    HANDLE pFile;//TXT文件指针

    machineFrequency mFre;//本机主频
    cycles_time ns[10];//存放10次获取rdtsc的数组

    inline cycles_time currentcycles();//获取当前的rdtsc
    inline void get10NS();//连续获取10个rdtsc
    inline machineFrequency getThisMachineFrequency();//获取本机主频

    int exTimes=0;//已经进行的实验次数
    int exDelay=0;//所有实验产生的间隔（每次实验产生9次间隔）

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
