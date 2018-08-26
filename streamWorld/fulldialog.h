#ifndef FULLDIALOG_H
#define FULLDIALOG_H

/************************************************************************
 * 全屏窗口
 * 注1.该窗口只用于播放视频时全屏使用，播放音频时无须全屏也无法全屏。
 * 注2.该窗口的全屏功能是Qt自带的fullScreen()函数实现的，匹配屏幕尺寸并且隐藏任务栏。
 * 注3.因为屏幕分辨率不固定，窗口中的控件位置是获取分辨率后再行设置的。
 * 注4.使用了事件过滤器eventFilter，通过悬停移动事件显示/隐藏播放控制条。
 * 注5.播放/暂停按钮位置是重叠的，使用showPlay()和showPause()来显示一个隐藏另一个。
 * 注6.全屏功能中未集成音量滑块和播放列表，感兴趣的朋友可以自行编写。
 *************************************************************************/

#include <QDialog>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <windows.h>

namespace Ui {
class fullDialog;
}

class fullDialog : public QDialog
{
    Q_OBJECT

public:
    explicit fullDialog(QWidget *parent = 0);
    ~fullDialog();

    WId getWinID();

    void setPlaying(bool);
    bool getPlaying();

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    Ui::fullDialog *ui;

    int fWidth;
    int fHeight;
    int ctrlHeight;

    bool isHide;

signals:
    //共同的播放功能。
    void play();
    void pause();
    void lastStation();
    void nextStation();
    void refreshStation();

    //转换为小窗口模式。
    void normalScreen();

private slots:
    //共同的播放/暂停功能。
    void showPlay();
    void showPause();
};

#endif // FULLDIALOG_H
