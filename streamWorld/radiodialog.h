#ifndef RADIODIALOG_H
#define RADIODIALOG_H

/******************************************************************
 * 音频窗口
 * 注1.音频窗口提供界面与控件对音频进行播放、暂停等操作，是通过给System发信号进
 *     行间接操作的。
 * 注2.音频窗口与视频窗口的区别：1.无视频播放界面；2.音频窗口有频道标签，更换频道
 *     时会显示相应频道名，视频窗口则没有。
 * 注3.频道标签是滚动显示的，频道名过长就会自动滚动。该功能使用了定时器。具体实现
 *     请参考timerStart()和scrollTitle()方法。
 * 注4.该窗口是无边框的自定义窗口，无法使用windows原生的拖动功能，因此通过事件过
 *     滤器捕获鼠标的拖动事件计算新旧位置的差值，进行移动，从而实现拖动效果。
 ******************************************************************/

#include <QDialog>
#include <QMouseEvent>
#include <QTimer>
#include <windows.h>

namespace Ui {
class radioDialog;
}

class radioDialog : public QDialog
{
    Q_OBJECT

public:
    explicit radioDialog(QWidget *parent = 0);
    ~radioDialog();

    void setPlaying(bool);     //设置播放状态的方法。
    void setVolume(int);       //设置音量的方法。

    void setTitle(QString);    //设置频道标题的方法。

private:
    Ui::radioDialog *ui;

    bool isDrag;               //这三个变量用于无边框窗体的移动。
    QPoint mouse;
    QPoint win;

    QTimer *timer;             //定时器，用于标题滚动。
    QString title;             //存储标题的变量。

protected:
    bool eventFilter(QObject *, QEvent *);

signals:
    //共同的播放功能。
    void play();
    void pause();
    void lastStation();
    void nextStation();
    void refreshStation();
    void stop();
    void changeVolume(int);

    //转换为视频模式。
    void showVideo();

    //打开设置、列表窗口。
    void showSettings();//设置窗口。
    void showList();    //列表窗口。

    //发送记录窗口位置的信号。
    void recordPos();
    //发送窗口移动的差值。
    void moveDif(QPoint);

    //最小化信号。
    void needMinSize();
    //关闭信号。
    void needClose();

private slots:
    //切换播放/暂停按钮显示的方法。
    void showPause();
    void showPlay();

public slots:
    //滚动标题的槽。
    void timerStart();   //启动定时器。
    void scrollTitle();  //滚动方法。
};

#endif // RADIODIALOG_H
