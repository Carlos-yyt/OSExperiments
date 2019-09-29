#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "windows.h"
#include <qdebug.h>
#define TXT_FILE_NAME "19317119_win32.txt"
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
private:
    char* path;//当前文件地址
    QString pathStr;//当前文件地址--QString

    char* pathTXT;//TXT文件的位置
    QString pathTXTStr;//当前文本文件地址--QString
    WCHAR pathTXTWChar[MAX_PATH];


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
