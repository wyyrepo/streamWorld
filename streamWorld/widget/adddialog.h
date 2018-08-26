#ifndef ADDDIALOG_H
#define ADDDIALOG_H

/*********************************************************************
 * 增加频道/组窗口
 * 注1.该窗口是列表窗口的子窗口，用于添加频道或者组使用。
 * 注2.使用了自定义安装的字体。
 * 信号
 * addGroupOrStation(QString, QString)
 * 当用户填写了正确的组/频道信息，该窗口将向列表窗口发信号产生一个新的组/频道。
 * 第一个参数为组/频道名；为了区分组和频道，组的第二个参数，即频道地址为空。
 * 槽
 * stationInvalid()  用户点选了添加组，添加频道区域便会失效
 * groupInvalid()    用户点选了添加频道，添加组区域便会失效
 * enter()           确定方法，填写信息完整就会发送add信号，不完整就弹出错误提示
 * cancel()          取消方法，清空已经填写的内容并退出该窗口
 **********************************************************************/

#include <QDialog>
#include <QMessageBox>
#include "Model/stationlist.h"

namespace Ui {
class addDialog;
}

class addDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addDialog(QWidget *parent = 0);
    ~addDialog();

private:
    Ui::addDialog *ui;

signals:
    void addGroupOrStation(QString, QString);

private slots:
    void stationInvalid();
    void groupInvalid();

    void enter();
    void cancel();
};

#endif // ADDDIALOG_H
