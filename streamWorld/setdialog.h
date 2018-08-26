#ifndef SETDIALOG_H
#define SETDIALOG_H

/**************************************************************
 * 设置窗口
 * 注1.该窗口用于显示、修改配置设置，但具体的配置生效是在System类中实现的
 *     (除了开机启动，开机启动是设置窗口实现的)。
 * 注2.开机启动功能是通过修改注册表实现的，所以在win7等系统中需要以管理员身
 *     份运行才有效，如果不这样启动的话其他功能不会受到影响。
 * 注3.该类通过读取/保存settings.ini文件进行配置设置，如果读取时配置文件不
 *     存在，程序会生成一个默认配置文件，该文件的内容预置于资源文件中。
 * 注4.重写关闭事件的原因请参考"关于窗口"(注1)。
 **************************************************************/

#include <QDialog>
#include <QSettings>
#include <QRadioButton>
#include <QLineEdit>
#include <QCloseEvent>
#include <Tools/textio.h>
#include <QDir>

namespace Ui {
class setDialog;
}

class setDialog : public QDialog
{
    Q_OBJECT

public:
    explicit setDialog(QWidget *parent = 0);
    ~setDialog();

    QRadioButton *radio;
    QRadioButton *video;
    QRadioButton *normal;
    QRadioButton *big;
    QRadioButton *task;
    QRadioButton *tray;
    QRadioButton *yes;
    QRadioButton *no;
    QLineEdit *lineEdit;

    void saveSettings();             //保存配置的方法。

    void closeEvent(QCloseEvent *);  //重写关闭事件。

private:
    Ui::setDialog *ui;
};

#endif // SETDIALOG_H
