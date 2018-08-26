#ifndef VIDEODIALOG_H
#define VIDEODIALOG_H

/*********************************************************************
 * 视频窗口
 * 注1.为VLC设置视频输出窗口，需要目标窗口的ID。在本程序中输出窗口是视频窗口的一个
 *     子控件(QWidget的对象displayWidget)。经实验，任何继承自QWidget的类都可
 *     以给VLC播放视频用，比如QLabel。
 * 注2.为了使程序功能更丰富，预设了大小两个视频窗口的布局，存储在资源文件的size.ini
 *     中，通过方法changeSize(bool)可以随时切换，但是本程序中仅在程序启动时设置
 *     该尺寸，并且没有运行时更改的按钮。感兴趣的朋友可以自行修改。
 * 注3.为了使显示/隐藏播放控制条更美观，使用了Qt中的属性动画QPropertyAnimation。
 * 注4.无边框窗体的移动问题请参考"音频窗口"(注4)。
 ********************************************************************/

#include <QDialog>
#include <QMouseEvent>
#include <QBitmap>
#include <QPropertyAnimation>
#include <QSettings>

namespace Ui {
class videoDialog;
}

class videoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit videoDialog(QWidget *parent = 0);
    ~videoDialog();

    //获得显示视频的窗口ID。
    WId getWinID();

    //设置暂停/播放按钮状态的方法。
    void setPlaying(bool);
    bool getPlaying();

    //设置音量的方法。
    void setVolume(int);

    //改变尺寸的方法：普通/大。
    void changeSize(bool normal = true);

private:
    Ui::videoDialog *ui;

    //无边框移动的参照变量。
    bool isDrag;
    QPoint mouse;
    QPoint win;

    //记录窗口大小模式的布尔变量。
    bool isNormal;

    //记录操作条尺寸的变量。
    QRect visibleSize;

protected:
    bool eventFilter(QObject *, QEvent *);

signals:
    //共同的播放功能。
    void play();
    void pause();
    void lastStation();
    void nextStation();
    void refreshStation();
    void changeVolume(int);

    //转换模式的信号。
    void showFull();    //转换为全屏模式。
    void showRadio();   //转换为音频模式。

    //打开设置、播放列表窗口的信号。
    void showList();    //列表窗口。
    void showSettings();//设置窗口。

    //发送记录窗口位置的信号。
    void recordPos();
    //发送窗口移动的差值。
    void moveDif(QPoint);

    //最小化信号。
    void needMinSize();
    //关闭信号。
    void needClose();

private slots:
    //共同的设置播放/暂停的槽。
    void showPlay();
    void showPause();

    //video中播放控制条的显示与隐藏。
    void changeActionBar();
};

#endif // VIDEODIALOG_H
