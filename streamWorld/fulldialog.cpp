#include "fulldialog.h"
#include "ui_fulldialog.h"
#include <QDebug>

fullDialog::fullDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fullDialog)
{
    ui->setupUi(this);

    ui->ctrlWidget->setMouseTracking(true);
    ui->ctrlWidget->installEventFilter(this);
    ui->disWidget->setAttribute(Qt::WA_TransparentForMouseEvents);

    isHide = true;
    ctrlHeight = 60;
    fWidth = QApplication::desktop()->width();
    fHeight = QApplication::desktop()->height();

    //根据屏幕大小设置控件位置。
    this->resize(QApplication::desktop()->size());

    ui->disWidget->setGeometry(0,0, fWidth, fHeight);
    ui->ctrlWidget->setGeometry(0,fHeight-1, fWidth,ctrlHeight);

    //设置浮动条中的控件布局。
    int objectY = (ui->ctrlWidget->height()-ui->lastButton->height()) / 2;
    int objectW = ui->lastButton->width();
    int space = (this->width() - objectW*5) / 15;
    ui->lastButton->move(space*5, objectY);
    ui->pauseButton->move(space*6+objectW, objectY);
    ui->playButton->move(ui->pauseButton->pos());
    ui->nextButton->move(space*7+objectW*2, objectY);
    ui->refreshButton->move(space*9+objectW*3, objectY);
    ui->fullButton->move(space*10+objectW*4, objectY);

    /********信号与槽********/
    //上个电台
    connect(ui->lastButton, SIGNAL(clicked(bool)), this, SIGNAL(lastStation()));
    connect(ui->lastButton, SIGNAL(clicked(bool)), this, SLOT(showPause()));
    //暂停
    connect(ui->pauseButton, SIGNAL(clicked(bool)), this, SIGNAL(pause()));
    connect(ui->pauseButton, SIGNAL(clicked(bool)), this, SLOT(showPlay()));
    //播放
    connect(ui->playButton, SIGNAL(clicked(bool)), this, SIGNAL(play()));
    connect(ui->playButton, SIGNAL(clicked(bool)), this, SLOT(showPause()));
    //下个电台
    connect(ui->nextButton, SIGNAL(clicked(bool)), this, SIGNAL(nextStation()));
    connect(ui->nextButton, SIGNAL(clicked(bool)), this, SLOT(showPause()));
    //刷新
    connect(ui->refreshButton, SIGNAL(clicked(bool)), this, SIGNAL(refreshStation()));
    connect(ui->refreshButton, SIGNAL(clicked(bool)), this, SLOT(showPause()));
    //退出全屏
    connect(ui->fullButton, SIGNAL(clicked(bool)), SIGNAL(normalScreen()));
}

fullDialog::~fullDialog()
{
    delete ui;
}

WId fullDialog::getWinID()
{
    return ui->disWidget->winId();
}

void fullDialog::setPlaying(bool playing)
{
    if (playing)
        this->showPause();
    else
        this->showPlay();
}

bool fullDialog::getPlaying()
{
    if (ui->playButton->isHidden())
        return true;
    else
        return false;
}

bool fullDialog::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mousevent = static_cast<QMouseEvent *>(event);
        if (isHide)
        {      
            QPropertyAnimation *animation = new QPropertyAnimation(ui->ctrlWidget, "pos");
            animation->setDuration(200);
            animation->setStartValue(ui->ctrlWidget->pos());
            animation->setEndValue(QPoint(0, fHeight-ctrlHeight));
            animation->start();
            Sleep(200);
            isHide = false;
        }
        else
        {
            if (mousevent->y()>0 && mousevent->y()<20)
            {
                QPropertyAnimation *animation = new QPropertyAnimation(ui->ctrlWidget, "pos");
                animation->setDuration(200);
                animation->setStartValue(ui->ctrlWidget->pos());
                animation->setEndValue(QPoint(0, fHeight-1)); 
                animation->start();
                Sleep(200);
                isHide = true;
            }
        }
    }

    return QDialog::eventFilter(target, event);
}

void fullDialog::showPlay()
{
    ui->playButton->show();
    ui->pauseButton->hide();
}

void fullDialog::showPause()
{
    ui->pauseButton->show();
    ui->playButton->hide();
}
