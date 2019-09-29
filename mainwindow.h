#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "windows.h"
#include <qdebug.h>

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
    void on_pushButton_clicked(bool checked);

    void on_isExistBtn_clicked(bool checked);

    void on_delExistBtn_clicked(bool checked);
    char* path;//当前文件地址
    QString pathStr;//当前文件地址的String
    char* pathTXT;//TXT文件的位置
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
