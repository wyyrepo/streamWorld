#ifndef SYSTEM_H
#define SYSTEM_H

/*************************************************************************************************
 * 系统类（功能管理）
 * 注1.程序的结构
 *                                  ___---后台（媒体载入、播放控制）---vlc
 *tray(托盘控制)---System(连接、管理)--------数据（存储频道，增删操作）---Station/Group/List
 *                                  ˉˉˉ---界面（播放媒体，读写配置）---radio/video/fullscreen/list/set
 * 注2.符合面向对象的思想，界面类只显示，数据类只存储。vlc负责后台，所有的类通过System的信号-槽机制交流。
 * 注3.关闭函数saveExit()中保存了配置设置和频道列表到文件，并释放了vlc和所有窗口。
 *************************************************************************************************/

#include <QObject>
#include "vlc.h"
#include "videodialog.h"
#include "radiodialog.h"
#include "fulldialog.h"
#include "setdialog.h"
#include "listdialog.h"
#include "widget/systemtray.h"

class System : public QObject
{
    Q_OBJECT
public:
    explicit System(QObject *parent = 0);

    ~System();

private:
    //vlc指针。
    Vlc *vlc;

    //五个窗口指针。
    videoDialog *video;
    radioDialog *radio;
    fullDialog *screen;
    setDialog *setDock;
    listDialog *listDock;

    bool isRadio;    //标志当前窗口模式，音频还是视频。
    bool isTask;     //标志后台模式，任务栏还是系统托盘。

    SystemTray *tray;//系统托盘指针，只有当后台模式为系统托盘时可用。

private slots:
    /*------共同的播放功能------*/
    void last();
    void next();
    void play();
    void pause();
    void SetVolume(int);
    void refresh();
    void stop();

    /*------模式转换方法------*/
    void showVideo();     //音频转视频。
    void showRadio();     //视频转音频。
    void fullScreen();    //视频转全屏。
    void normalScreen();  //全屏转视频。

    /*------系统托盘的操作------*/
    //点击托盘图标显示或隐藏音频/视频窗口的槽。
    void trayActivated(QSystemTrayIcon::ActivationReason);
    void radioTray();    //打开音频窗口的槽。
    void videoTray();    //打开视频窗口的槽。

    /*------列表窗口操作------*/
    void showList();    //显示列表窗口。
    void listPlay(QString name, QString address);  //从列表窗口中点击以播放频道的方法。
    //列表窗口对音频窗口的跟随。
    void listFromRadio();
    //列表窗口对视频窗口的跟随。
    void listFromVideo();

    /*------设置窗口操作------*/
    void showSettings();//显示设置操作。

    /*------最小化&关闭方法------*/
    //音频/视频窗口最小化的操作，其中列表窗口也要跟着最小化。
    void minSize();
    //保存并关闭的方法，保存配置和频道列表。
    void saveExit();
};

#endif // SYSTEM_H
