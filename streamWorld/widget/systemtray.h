#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

/****************************************************************************
 * 系统托盘类
 * 注1.用户设置了后台模式为系统托盘，该类才有效。而且修改后台模式是下次启动生效的。
 * 注2.托盘的右键菜单有四个功能：音频/视频/关于/退出，前三个是弹出窗口，其中音频和视频窗口
 *     该类无权管理，只是发送信号要求System类进行弹出，关于窗口是托盘类的子窗口，可弹出。
 ****************************************************************************/

#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include "aboutdialog.h"

class SystemTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    SystemTray(QObject *);

    QAction *radioAction;   //音频功能，后台模式设置为系统托盘时可用。
    QAction *videoAciton;   //视频功能，后台模式设置为系统托盘时可用。
    QAction *aboutAction;   //关于功能，后台模式设置为系统托盘时可用。
    QAction *quitAciton;    //退出功能，后台模式设置为系统托盘时可用。
    QMenu *trayMenu;        //功能菜单。

private:
    aboutDialog *about;     //关于窗口。
};

#endif // SYSTEMTRAY_H
