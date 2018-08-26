#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

/***********************************************************
 * 关于窗口
 * 注1.带有原生标题栏的窗口，必须重写关闭事件，将close()改为hide()，
 *     因为这种窗口被Qt视为主窗口，关闭这种窗口Qt会认为程序已经没有可视
 *     窗口了，从而关闭整个程序。
 * 注2.该窗口是系统托盘的子窗口，只在系统托盘模式下有效。
 * 注3.该窗口使用了自定义安装的字体，理论上与系统无关，都能正常显示。
 ***********************************************************/

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class aboutDialog;
}

class aboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit aboutDialog(QWidget *parent = 0);
    ~aboutDialog();

    void closeEvent(QCloseEvent *);

private:
    Ui::aboutDialog *ui;
};

#endif // ABOUTDIALOG_H
