#include "videodialog.h"
#include "ui_videodialog.h"
#include <QDebug>

videoDialog::videoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::videoDialog)
{
    ui->setupUi(this);

    /********基本属性设置********/
    setWindowFlags(Qt::FramelessWindowHint);  //取消标题栏。
    ui->backLabel->installEventFilter(this);  //为播放器背景安装事件过滤器,用于移动窗口。
    ui->titleLabel->setAttribute(Qt::WA_TransparentForMouseEvents);    //“流视界”标签不接受事件，方便窗口移动。
    ui->backLabel->setMouseTracking(true);    //设置背景标签可以捕捉鼠标悬停事件，用于显示/隐藏播放控制条。
    ui->displayWidget->setAttribute(Qt::WA_TransparentForMouseEvents); //播放控件不接受事件，方便操作处理。

    //初始化拖动的标志变量。
    isDrag = false;

    /********连接信号槽********/
    //最小化&最大化&关闭
    connect(ui->minButton, SIGNAL(clicked(bool)), this, SIGNAL(needMinSize()));
    connect(ui->maxButton, SIGNAL(clicked(bool)), this, SIGNAL(showFull()));
    connect(ui->closeButton, SIGNAL(clicked(bool)), this, SIGNAL(needClose()));

    //侧边栏--左侧：音频/设置；右侧：列表/显示
    connect(ui->radioButton, SIGNAL(clicked(bool)), this, SIGNAL(showRadio()));
    connect(ui->setButton, SIGNAL(clicked(bool)), this, SIGNAL(showSettings()));
    connect(ui->listButton, SIGNAL(clicked(bool)), this, SIGNAL(showList()));
    connect(ui->showButton, SIGNAL(clicked(bool)), this, SLOT(changeActionBar()));

    //播放控制条：上一台、播放/暂停、下一台、刷新、音量滑块
    //上一台
    connect(ui->lastButton, SIGNAL(clicked(bool)), this, SIGNAL(lastStation()));
    connect(ui->lastButton, SIGNAL(clicked(bool)), this, SLOT(showPause()));
    //暂停
    connect(ui->pauseButton, SIGNAL(clicked(bool)), this, SIGNAL(pause()));
    connect(ui->pauseButton, SIGNAL(clicked(bool)), this, SLOT(showPlay()));
    //播放
    connect(ui->playButton, SIGNAL(clicked(bool)), this, SIGNAL(play()));
    connect(ui->playButton, SIGNAL(clicked(bool)), this, SLOT(showPause()));
    //下一台
    connect(ui->nextButton, SIGNAL(clicked(bool)), this, SIGNAL(nextStation()));
    connect(ui->nextButton, SIGNAL(clicked(bool)), this, SLOT(showPause()));
    //刷新
    connect(ui->refreshButton, SIGNAL(clicked(bool)), this, SIGNAL(refreshStation()));
    connect(ui->refreshButton, SIGNAL(clicked(bool)), this, SLOT(showPause()));
    //调节音量
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)),
            this, SIGNAL(changeVolume(int)));
}

videoDialog::~videoDialog()
{
    delete ui;
}

WId videoDialog::getWinID()
{
    return ui->displayWidget->winId();
}

void videoDialog::setPlaying(bool playing)
{
    if (playing)
        this->showPause();
    else
        this->showPlay();
}

bool videoDialog::getPlaying()
{
    if (ui->playButton->isHidden())
        return true;
    else
        return false;
}

//事件过滤器的实现。
bool videoDialog::eventFilter(QObject *target, QEvent *event)
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

    return QDialog::eventFilter(target, event);
}

void videoDialog::showPlay()
{
    ui->playButton->show();
    ui->pauseButton->hide();
}

void videoDialog::showPause()
{
    ui->pauseButton->show();
    ui->playButton->hide();
}

void videoDialog::setVolume(int v)
{
    ui->volumeSlider->setValue(v);
}

void videoDialog::changeSize(bool normal)
{
    isNormal = normal;

    QString model = "normal/";
    if (!isNormal)
        model = "big/";

    //读取尺寸并设置。
    QSettings *config = new QSettings("://size.ini", QSettings::IniFormat);

    this->resize(config->value("/"+model+"this").toSize());
    ui->backLabel->setGeometry(config->value(model+"back").toRect());

    ui->titleLabel->setGeometry(config->value(model+"title").toRect());
    ui->minButton->setGeometry(config->value(model+"min").toRect());
    ui->maxButton->setGeometry(config->value(model+"max").toRect());
    ui->closeButton->setGeometry(config->value(model+"close").toRect());

    ui->radioButton->setGeometry(config->value(model+"radio").toRect());
    ui->setButton->setGeometry(config->value(model+"set").toRect());
    ui->listButton->setGeometry(config->value(model+"list").toRect());
    ui->showButton->setGeometry(config->value(model+"show").toRect());

    ui->displayWidget->setGeometry(config->value(model+"display").toRect());
    ui->actionWidget->setGeometry(config->value(model+"actionWidget").toRect());
    ui->lastButton->setGeometry(config->value(model+"last").toRect());
    ui->pauseButton->setGeometry(config->value(model+"pause").toRect());
    ui->playButton->setGeometry(config->value(model+"play").toRect());
    ui->nextButton->setGeometry(config->value(model+"next").toRect());
    ui->refreshButton->setGeometry(config->value(model+"refresh").toRect());
    ui->bottomLabel->setGeometry(config->value(model+"bottom").toRect());
    ui->volumeSlider->setGeometry(config->value(model+"volume").toRect());
    ui->icoLabel->setGeometry(config->value(model+"ico").toRect());
    ui->actionHider->setGeometry(config->value(model+"actionHider").toRect());

    visibleSize = config->value(model+"actionWidget").toRect();

    //读取结束。
    delete config;

    //设置窗口遮罩。
    QBitmap map("://图片/videoMask.png");
    map = map.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    setMask(map);
}

void videoDialog::changeActionBar()
{
    QPropertyAnimation *flash = new QPropertyAnimation(ui->actionWidget, "geometry");
    if (ui->actionWidget->height() < 15)    //隐藏了
    {
        flash->setStartValue(ui->actionWidget->geometry());
        flash->setEndValue(visibleSize);
        flash->setDuration(200);
        flash->start();
    }
    else
    {
        flash->setStartValue(ui->actionWidget->geometry());
        flash->setEndValue(ui->actionHider->geometry());
        flash->setDuration(200);
        flash->start();
    }
}
