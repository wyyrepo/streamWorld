#include "systemtray.h"

SystemTray::SystemTray(QObject *parent)
{
    this->setParent(parent);

    setIcon(QIcon("://myico.ico"));
    this->setToolTip("流视界");
    //初始化三个功能。
    radioAction = new QAction(QIcon("://图片/图标/radio.ico"), "音频", this);
    videoAciton = new QAction(QIcon("://图片/图标/video.ico"), "视频", this);
    aboutAction = new QAction(QIcon("://图片/图标/about.ico"), "关于", this);
    quitAciton = new QAction(QIcon("://图片/图标/quit.ico"), "退出", this);
    //初始化功能菜单。
    trayMenu = new QMenu();
    trayMenu->addAction(radioAction);
    trayMenu->addAction(videoAciton);
    trayMenu->addSeparator();
    trayMenu->addAction(aboutAction);
    trayMenu->addAction(quitAciton);
    //给托盘设置菜单。
    this->setContextMenu(trayMenu);

    about = new aboutDialog();

    connect(aboutAction, SIGNAL(triggered(bool)), about, SLOT(show()));
}
