#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "windows.h"
#include <qdebug.h>
#include <QString>
#include <QDateTime>
#include <QTime>
#define TXT_FILE_NAME "19317119_win32.txt"
typedef unsigned long long cycles_time;//用来存放64位的时间戳计数器里面的内容
typedef unsigned long long machineFrequency;//用来存放机器主频
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_delExistBtn_clicked(bool checked);

    bool charToWChar(char* src,WCHAR *dest);//char字符串向WChar转换
    void on_createFileBtn_clicked(bool checked);

    void on_writeTXTBtn_clicked(bool checked);

private:
    char path[MAX_PATH];//当前文件地址
    QString pathStr;//当前文件地址--QString

    char pathTXT[MAX_PATH];//TXT文件的位置
    QString pathTXTStr;//当前文本文件地址--QString
    WCHAR pathTXTWChar[MAX_PATH];

    HANDLE pFile;//TXT文件指针

    inline cycles_time currentcycles();//获取一次当前的时间戳

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
