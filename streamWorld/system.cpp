#include "system.h"
#include <QDebug>

System::System(QObject *parent) : QObject(parent)
{
    /**********vlc初始化**********/
    vlc = new Vlc();

    /**********窗口初始化**********/
    radio = new radioDialog();
    video = new videoDialog();
    screen = new fullDialog();
    setDock = new setDialog();    //已经读取配置并更新显示。
    listDock = new listDialog();  //已经读取列表并更新显示。

    /*********配置设置**********/
    //根据设置内容进行处理。
    //第二项：设置视频窗口尺寸。这是唯一的初始化机会，后面不再调整。所以放到最前面。
    if (setDock->normal->isChecked())
        video->changeSize(true);
    else
        video->changeSize(false);

    //第三项：设置后台模式，程序初始为任务栏模式，如果用户修改为托盘模式，以下进行处理。
    if (!setDock->task->isChecked())
    {
        //设置后台标志。
        isTask = false;
        //取消视频窗口，音频窗口在任务栏中的显示。
        video->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
        radio->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
        //初始化系统托盘。
        tray = new SystemTray(this);
        //显示托盘图标。
        tray->show();
    }
    else    //任务栏模式下的设置。
    {
        //设置后台标志。
        isTask = true;
        tray = NULL;    //任务栏模式下，安全设置tray为空指针，防止出错。
    }

    //第一项：设置启动默认模式。
    if (setDock->radio->isChecked())   //音频模式初始化。
    {
        //显示音频窗口。
        radio->show();
        //列表窗口部分。
        listDock->changeModel(false);  //变更列表模式为音频频道。
        listFromRadio();               //移动列表窗口到合适位置。
        //更新音频标题显示。
        radio->setTitle(listDock->getFirstName());
        //vlc载入第一个频道。
        vlc->setMedia(listDock->getFirstAdd(), 0);
        //设置标志。
        isRadio = true;
    }
    else                                //视频模式初始化。
    {
        //显示视频窗口。
        video->show();
        //列表窗口部分。
        listDock->changeModel(true);    //变更列表模式为视频频道。
        listFromVideo();                //移动列表窗口到合适位置。
        //vlc载入第一个频道。
        vlc->setMedia(listDock->getFirstAdd(), video->getWinID());
        //设置标志。
        isRadio = false;
    }

    //第五项：设置缓冲时间。
    vlc->setCache(setDock->lineEdit->text().toInt());

    /**********信号与槽**********/
    /*-----音频界面-----*/
    //播放功能。
    connect(radio, SIGNAL(lastStation()), this, SLOT(last()));
    connect(radio, SIGNAL(nextStation()), this, SLOT(next()));
    connect(radio, SIGNAL(pause()), this, SLOT(pause()));
    connect(radio, SIGNAL(play()), this, SLOT(play()));
    connect(radio, SIGNAL(refreshStation()), this, SLOT(refresh()));
    connect(radio, SIGNAL(stop()), this, SLOT(stop()));
    connect(radio, SIGNAL(changeVolume(int)), this, SLOT(SetVolume(int)));
    //模式转换。
    connect(radio, SIGNAL(showVideo()), this, SLOT(showVideo()));
    //显示设置/列表窗口。
    connect(radio, SIGNAL(showSettings()), this, SLOT(showSettings()));
    connect(radio, SIGNAL(showList()), this, SLOT(showList()));
    //列表窗口的跟随。
    connect(radio, SIGNAL(recordPos()), listDock, SLOT(recordPos()));
    connect(radio, SIGNAL(moveDif(QPoint)), listDock, SLOT(moveDif(QPoint)));
    //最小化窗口。
    connect(radio, SIGNAL(needMinSize()), this, SLOT(minSize()));
    //关闭程序。
    connect(radio, SIGNAL(needClose()), this, SLOT(saveExit()));

    /*-----视频界面-----*/
    //播放功能。
    connect(video, SIGNAL(lastStation()), this, SLOT(last()));
    connect(video, SIGNAL(nextStation()), this, SLOT(next()));
    connect(video, SIGNAL(play()), this, SLOT(play()));
    connect(video, SIGNAL(pause()), this, SLOT(pause()));
    connect(video, SIGNAL(refreshStation()), this, SLOT(refresh()));
    connect(video, SIGNAL(changeVolume(int)), this, SLOT(SetVolume(int)));
    //模式转换。
    connect(video, SIGNAL(showRadio()), this, SLOT(showRadio()));
    connect(video, SIGNAL(showFull()),this, SLOT(fullScreen()));
    //显示设置/列表窗口。
    connect(video, SIGNAL(showSettings()), this, SLOT(showSettings()));
    connect(video, SIGNAL(showList()), this, SLOT(showList()));
    //列表窗口的跟随。
    connect(video, SIGNAL(recordPos()), listDock, SLOT(recordPos()));
    connect(video, SIGNAL(moveDif(QPoint)), listDock, SLOT(moveDif(QPoint)));
    //最小化窗口。
    connect(video, SIGNAL(needMinSize()), this, SLOT(minSize()));
    //关闭程序。
    connect(video, SIGNAL(needClose()), this, SLOT(saveExit()));

    /*-----全屏界面-----*/
    //播放功能。
    connect(screen, SIGNAL(lastStation()), this, SLOT(last()));
    connect(screen, SIGNAL(nextStation()), this, SLOT(next()));
    connect(screen, SIGNAL(play()), this, SLOT(play()));
    connect(screen, SIGNAL(pause()), this, SLOT(pause()));
    connect(screen, SIGNAL(refreshStation()), this, SLOT(refresh()));
    //模式转换。
    connect(screen, SIGNAL(normalScreen()),this, SLOT(normalScreen()));

    /*-----列表界面-----*/
    connect(listDock, SIGNAL(listPlay(QString, QString)), this, SLOT(listPlay(QString,QString)));

    /*-----设置界面-----*/
    //尚无内容

    /*-----系统托盘-----*/
    if (!isTask)   //托盘模式下，这部分信号槽有效。
    {
        //连接点击图标的信号与槽，进行音频/视频窗口的显示与隐藏。
        connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
        //打开音频窗口。
        connect(tray->radioAction, SIGNAL(triggered(bool)), this, SLOT(radioTray()));
        //打开视频窗口。
        connect(tray->videoAciton, SIGNAL(triggered(bool)), this, SLOT(videoTray()));
        //退出程序。
        connect(tray->quitAciton, SIGNAL(triggered(bool)), this, SLOT(saveExit()));
    }

}

System::~System()
{
}

void System::last()
{
    char *add = vlc->getUrl();                       //获得当前频道地址。
    QString station = listDock->lastStation(add);    //获得上一个频道的地址。

    QByteArray ba = station.toLatin1();
    char *last = ba.data();                          //获得char ×类型的地址。

    if (isRadio)
    {
        radio->setTitle(listDock->findName(last));    //音频模式需要设置标题。
        radio->timerStart();                          //标题滚动。
        vlc->setMedia(last, 0);
    }
    else
        vlc->setMedia(last, video->getWinID());

    vlc->play();    //播放。
}

void System::next()
{
    char *add = vlc->getUrl();                       //获得当前频道地址。
    QString station = listDock->nextStation(add);    //获得下一个频道的地址。

    QByteArray ba = station.toLatin1();
    char *next = ba.data();                          //获得char ×类型的地址。

    if (isRadio)
    {
        radio->setTitle(listDock->findName(next));    //音频模式需要设置标题。
        radio->timerStart();                          //标题滚动。
        vlc->setMedia(next, 0);
    }
    else
        vlc->setMedia(next, video->getWinID());

    vlc->play();    //播放。
}

void System::play()
{
    vlc->play();
}

void System::pause()
{
    vlc->pause();
}

void System::SetVolume(int v)
{
    vlc->setVolume(v);
}

void System::refresh()
{
    vlc->stop();
    vlc->play();
}

void System::stop()
{
    vlc->stop();
}

void System::showVideo()
{
    //停止播放。
    vlc->stop();
    radio->setPlaying(false);
    //隐藏音频窗口。
    radio->hide();
    //显示视频窗口。
    video->show();
    //转换列表内容。
    listDock->changeModel(true);
    //调整列表的位置。
    listFromVideo();
    //将列表第一项载入vlc。
    vlc->setMedia(listDock->getFirstAdd(), video->getWinID());
    //设置标志。
    isRadio = false;
}

void System::showRadio()
{
    //停止播放。
    vlc->stop();
    video->setPlaying(false);
    //隐藏视频窗口。
    video->hide();
    //显示音频窗口。
    radio->show();
    //转换列表内容。
    listDock->changeModel(false);
    //调整列表位置。
    listFromRadio();
    //设置音频标签。
    radio->setTitle(listDock->getFirstName());
    //将列表第一项载入vlc。
    vlc->setMedia(listDock->getFirstAdd(), 0);
    //设置标志。
    isRadio = true;
}

void System::fullScreen()
{
    //停止播放。
    vlc->stop();
    //隐藏video。
    video->hide();
    //显示screen并进行播放。
    screen->showFullScreen();
    vlc->setId(screen->getWinID());
    vlc->play();
    //设置screen的按钮。
    screen->setPlaying(true);
}

void System::normalScreen()
{
    //停止播放。
    vlc->stop();
    //隐藏screen。
    screen->hide();
    //显示video并进行播放。
    video->show();
    vlc->setId(video->getWinID());
    vlc->play();
    //设置video的按钮。
    video->setPlaying(true);
}

void System::minSize()
{
    if (isTask)    //任务栏模式下，窗口进行最小化。
    {
        if (isRadio)
            radio->showMinimized();
        else
            video->showMinimized();

        listDock->hide();
    }
    else           //系统托盘模式下，窗口直接隐藏。
    {
        if (isRadio)
            radio->hide();
        else
            video->hide();

        listDock->hide();
    }
}

void System::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
    {
        if (isRadio)
        {
            if (radio->isVisible())
            {
                radio->hide();
                listDock->hide();
            }
            else
                radio->show();
        }
        else
        {
            if (video->isVisible())
            {
                video->hide();
                listDock->hide();
            }
            else
                video->show();
        }
    }
}

void System::radioTray()
{
    if (isRadio)
    {
        if (radio->isHidden())
            radio->show();
    }
    else
        showRadio();
}

void System::videoTray()
{
    if (isRadio)
        showVideo();
    else
    {
        if (video->isHidden())
            video->show();
    }
}

void System::listFromRadio()
{
    listDock->move(radio->pos()+QPoint(305,-100));
}

void System::listFromVideo()
{
    int x = video->x() + video->width() + 5;
    int y = video->y() + (video->height()-listDock->height())/2;

    listDock->move(x, y);
}

void System::showSettings()
{
    if (setDock->isHidden())
        setDock->show();
    else
        setDock->hide();
}

void System::showList()
{
    if (listDock->isHidden())
        listDock->show();
    else
        listDock->hide();
}

void System::listPlay(QString name, QString address)
{
    //获得char ×类型的地址。
    QByteArray ba = address.toLatin1();
    char *add = ba.data();

    //载入vlc。
    vlc->stop();

    if (radio->isVisible())
    {
        vlc->setMedia(add, 0);
        radio->setPlaying(true);
        radio->setTitle(name);
        radio->timerStart();
    }
    else
    {
        vlc->setMedia(add, video->getWinID());
        video->setPlaying(true);
    }

    vlc->play();
}

void System::saveExit()
{
    //保存配置设置。
    setDock->saveSettings();

    //保存频道列表。
    if (!listDock->writeList())
        qDebug()<<"写入频道文件失败！";

    //关闭vlc。
    delete vlc;

    //关闭各个窗口。
    video->close();
    radio->close();
    screen->close();
    setDock->close();
    listDock->close();
}
