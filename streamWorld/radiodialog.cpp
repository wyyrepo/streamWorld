#include "radiodialog.h"
#include "ui_radiodialog.h"
#include <QDebug>

radioDialog::radioDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::radioDialog)
{
    ui->setupUi(this);

    /********基本属性设置********/
    setWindowFlags(Qt::FramelessWindowHint);      //取消标题栏。
    setAttribute(Qt::WA_TranslucentBackground);   //隐藏窗体背景。

    ui->titleLabel->setAttribute(Qt::WA_TransparentForMouseEvents);  //设置标题不接受事件，便于窗口移动。
    ui->radioLabel->setAttribute(Qt::WA_TransparentForMouseEvents);  //设置电台标签不接受事件，便于窗口移动。
    ui->backLabel->installEventFilter(this);      //为播放器背景安装事件过滤器。

    //初始化拖动的标志变量。
    isDrag = false;

    //初始化定时器。
    timer = new QTimer(this);

    /********信号槽********/
    //定时器，用于滚动标题。
    connect(timer, SIGNAL(timeout()), this, SLOT(scrollTitle()));

    //最小化&关闭
    connect(ui->minButton, SIGNAL(clicked(bool)), this, SIGNAL(needMinSize()));
    connect(ui->closeButton, SIGNAL(clicked(bool)), this, SIGNAL(needClose()));

    //播放界面：播放/暂停、上一台、下一台、刷新、关闭、调节音量。
    //播放。
    connect(ui->playButton, SIGNAL(clicked(bool)), this, SIGNAL(play()));
    connect(ui->playButton, SIGNAL(clicked(bool)), this, SLOT(showPause()));
    connect(ui->playButton, SIGNAL(clicked(bool)), this, SLOT(timerStart()));  //播放时开启定时器。
    //暂停。
    connect(ui->pauseButton, SIGNAL(clicked(bool)), this, SIGNAL(pause()));
    connect(ui->pauseButton, SIGNAL(clicked(bool)), this, SLOT(showPlay()));
    //上一台。
    connect(ui->lastButton, SIGNAL(clicked(bool)), this, SIGNAL(lastStation()));
    connect(ui->lastButton, SIGNAL(clicked(bool)), this, SLOT(showPause()));
    //下一台。
    connect(ui->nextButton, SIGNAL(clicked(bool)), this, SIGNAL(nextStation()));
    connect(ui->nextButton, SIGNAL(clicked(bool)), this, SLOT(showPause()));
    //刷新。
    connect(ui->refreshButton, SIGNAL(clicked(bool)), this, SIGNAL(refreshStation()));
    connect(ui->refreshButton, SIGNAL(clicked(bool)), this, SLOT(showPause()));
    //关闭。
    connect(ui->stopButton, SIGNAL(clicked(bool)), this, SIGNAL(stop()));
    connect(ui->stopButton, SIGNAL(clicked(bool)), this, SLOT(showPlay()));
    //调节音量。
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), this, SIGNAL(changeVolume(int)));

    //侧边栏：视频、设置、列表
    connect(ui->videoButton, SIGNAL(clicked(bool)), this, SIGNAL(showVideo()));
    connect(ui->setButton, SIGNAL(clicked(bool)), this, SIGNAL(showSettings()));
    connect(ui->listButton, SIGNAL(clicked(bool)), this, SIGNAL(showList()));
}

radioDialog::~radioDialog()
{
    delete ui;
}

void radioDialog::setPlaying(bool playing)
{
    if (playing)
        showPause();
    else
        showPlay();
}

void radioDialog::setVolume(int v)
{
    ui->volumeSlider->setValue(v);
}

void radioDialog::setTitle(QString name)
{
    ui->radioLabel->setText(name);
}

//事件过滤器的实现，用于自由拖动窗口。
bool radioDialog::eventFilter(QObject *target, QEvent *event)
{
    if (target == ui->backLabel)
        {
            if (event->type() == QEvent::MouseButtonPress)
            {
                isDrag = true;
                QMouseEvent *mousevent = static_cast<QMouseEvent *>(event);
                //获取当前全局鼠标位置。
                mouse = cursor().pos();
                //获取窗口在屏幕的位置。
                win = mouse - mousevent->pos();
                //发送信号要求列表窗口记录位置。
                emit recordPos();
            }
            if (event->type() == QEvent::MouseMove && isDrag)
            {
                //获取当前全局鼠标位置。
                QPoint current = cursor().pos();
                //计算窗口在屏幕上的新位置。
                QPoint temp = win + current - mouse;
                //移动窗口位置。
                this->move(temp);
                //发送差值。
                QPoint dif = current - mouse;
                emit moveDif(dif);
            }
            if (event->type() == QEvent::MouseButtonRelease)
            {
                isDrag = false;
            }
        }

    return QDialog::eventFilter(target, event);
}

void radioDialog::showPause()
{
    ui->playButton->hide();
    ui->pauseButton->show();
}

void radioDialog::showPlay()
{
    ui->playButton->show();
    ui->pauseButton->hide();
}

void radioDialog::timerStart()
{
    if (ui->radioLabel->text().length() > 6)
    {
        title = ui->radioLabel->text();
        timer->start(800);
    }
    else
        timer->stop();
}

void radioDialog::scrollTitle()
{
    QString text = ui->radioLabel->text();
    text.remove(0, 1);
    ui->radioLabel->setText(text);

    if (text.length() == 5)
        ui->radioLabel->setText(title);
}
