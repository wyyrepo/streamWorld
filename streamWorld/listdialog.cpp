#include "listdialog.h"
#include "ui_listdialog.h"
#include <QDebug>

listDialog::listDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::listDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    //设置字体。
    QFont font;
    font.setFamily("FZCuHuoYi-M25S");

    //设置标题。
    font.setPointSize(11);
    ui->addButton->setFont(font);
    ui->modButton->setFont(font);
    ui->moveButton->setFont(font);
    ui->delButton->setFont(font);

    //初始化音频与视频列表。
    currentList = new StationList();
    radioList = new StationList();
    videoList = new StationList();
    //初始化播放列表。
    playList = new QStringList();

    //查看是否有音频列表文件，没有就生成一个。
    TextIO radio;
    if (!radio.isExist("radio.list"))
    {
        radio.openFile("radio.list", writeOnly);
        radio.inLine("groupName#音频直播");
        radio.inLine("AsiaFM 亚洲音乐台#rtsp://asiafm.cn/asiafm");
        radio.inLine("AsiaFM 亚洲天空台#rtsp://funradio.cn/funradio");
        radio.inLine("AsiaFM 亚洲经典台#rtsp://goldfm.cn/goldfm");
        radio.inLine("AsiaFM HD音乐台#rtsp://asiafm.cn/asiahd");
        radio.closeFile();
    }
    //查看是否有视频列表文件，没有就生成一个。
    TextIO video;
    if (!video.isExist("video.list"))
    {
        video.openFile("video.list", writeOnly);
        video.inLine("groupName#视频直播");
        video.inLine("CCTV1#http://cctv1.vtime.cntv.cloudcdn.net:8000/live/flv/channel14?AUTH=AW5SoCotUP+w/TliHwVgJxWnQe4CJLWylW1h63SGGfldR0dvYINQ+YcPTPTYgiLEsCwiqqApiI0WZx0DgCF4nw==");
        video.closeFile();
    }

    //读取列表。
    if (!readList())
        qDebug()<<"读取频道列表失败！";

    //默认音频模式。
    changeModel(false);

    //子窗口的初始化。
    addWidget = new addDialog(this);

    //信号与槽。
    connect(ui->minButton, SIGNAL(clicked(bool)), this, SLOT(hide()));
    connect(ui->closeButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(selectStation(QTreeWidgetItem*,int)));

    connect(ui->addButton, SIGNAL(clicked(bool)), addWidget, SLOT(show()));
    connect(addWidget, SIGNAL(addGroupOrStation(QString,QString)),
            this, SLOT(addGroupOrStation(QString,QString)));
    connect(ui->delButton, SIGNAL(clicked(bool)), this, SLOT(delGroupOrStation()));
}

listDialog::~listDialog()
{
    delete ui;
}

void listDialog::changeModel(bool videoModel)
{
    if (videoModel)
        currentList = videoList;
    else
        currentList = radioList;

    refreshList();
}

bool listDialog::readList()
{
    bool radio = radioList->fromFile("./radio.list");
    bool video = videoList->fromFile("./video.list");

    if (radio && video)
        return true;
    else
        return false;
}

bool listDialog::writeList()
{
    bool video = videoList->toFile("./video.list");
    bool radio = radioList->toFile("./radio.list");

    if (radio && video)
        return true;
    else
        return false;
}

void listDialog::refreshList()
{
    ui->treeWidget->clear();
    QIcon folderIcon(QPixmap("://图片/图标/folder.ico"));
    QIcon stationIcon;
    if (currentList == radioList)
        stationIcon.addPixmap(QPixmap("://图片/图标/radio.ico"));
    else
        stationIcon.addPixmap(QPixmap("://图片/图标/video.ico"));

    for (int i = 0; i < currentList->count(); i++)
    {
        StationGroup *group = currentList->getGroup(i);
        QTreeWidgetItem *groupItem = new QTreeWidgetItem(QStringList()<<group->getName());
        groupItem->setIcon(0, folderIcon);
        ui->treeWidget->addTopLevelItem(groupItem);

        for (int j = 0; j < group->count(); j++)
        {
            Station *station = group->getStation(j);
            QTreeWidgetItem *stationItem = new QTreeWidgetItem(QStringList()<<station->getName());
            stationItem->setIcon(0, stationIcon);
            groupItem->addChild(stationItem);
        }
    }

    setPlayList();
}

void listDialog::setPlayList()
{
    playList->clear();    //清空播放列表。

    for (int i = 0; i < currentList->count(); i++)
    {
        StationGroup *group = currentList->getGroup(i);
        for (int j = 0; j < group->count(); j++)
        {
            Station *station = group->getStation(j);
            playList->append(station->getAddress());
        }
    }
}

QString listDialog::lastStation(char *s)
{
    QString station(s);
    int pos = playList->indexOf(station) - 1;     //将当前频道的位置减一，得到上一个频道的位置。

    if (pos < 0)    //如果原频道是第一个，上一个频道就会为负值，此时循环至列表末位频道。
        pos = playList->count() - 1;              //设置为末位频道的位置。

    return playList->at(pos);
}

QString listDialog::nextStation(char *s)
{
    QString station(s);
    int pos = playList->indexOf(station) + 1;     //将当前频道的位置加一，得到下一个频道的位置。

    if (pos == playList->count())    //如果原频道是最后一个，下一个频道就会越界，此时循环至列表首位频道。
        pos = 0;                     //设置为首位频道的位置。

    return playList->at(pos);
}

QString listDialog::getFirstName()
{
    for (int i = 0; i < currentList->count(); i++)
        if (currentList->getGroup(i)->count() > 0)
        {
            QString first = currentList->getGroup(i)->getStation(0)->getName();

            return first;
        }

    return "error";
}

QString listDialog::findName(char *address)
{
    QString add(address);

    for (int i = 0; i < currentList->count(); i++)
    {
        StationGroup *group = currentList->getGroup(i);
        for (int j = 0; j < group->count(); j++)
        {
            Station *station = group->getStation(j);

            if (station->getAddress() == add)
                return station->getName();
        }
    }

    return "error";
}

char *listDialog::getFirstAdd()
{
    for (int i = 0; i < currentList->count(); i++)
        if (currentList->getGroup(i)->count() > 0)
        {
            QString first = currentList->getGroup(i)->getStation(0)->getAddress();
            QByteArray ba = first.toLatin1();
            return ba.data();
        }

    return NULL;
}

void listDialog::recordPos()
{
    record = this->pos();
}

void listDialog::moveDif(QPoint dif)
{
    this->move(record + dif);
}

void listDialog::selectStation(QTreeWidgetItem *item, int c)
{
    if (item->childCount() == 0)
    {
        QString name = item->text(c);
        for (int i = 0; i < currentList->count(); i++)
        {
            StationGroup *group = currentList->getGroup(i);
            for (int j = 0; j < group->count(); j++)
            {
                if (group->getStation(j)->getName() == name)
                    emit listPlay(name, group->getStation(j)->getAddress());
            }
        }
    }
}

void listDialog::addGroupOrStation(QString name, QString address)
{
    if (address.isEmpty())    //地址为空，表示收到的是组。
    {
        StationGroup *group = new StationGroup();
        group->setName(name);
        currentList->addGroup(group);

        refreshList();
    }
    else                      //收到的是频道。
    {
        //将收到的数据转化成频道指针。
        Station *station = new Station(name, address);
        //获得被选中的项。
        QTreeWidgetItem *item = ui->treeWidget->currentItem();
        if (item == NULL)     //没有项目被选中，加入默认的组。
        {
            StationGroup *group = currentList->getGroup(0);
            group->addStation(station);
        }
        else                  //获得这个组，并将频道插入。
        {
            if (item->parent())    //该项有父项，说明选择的是频道，那就找到他的组。
                item = item->parent();          //获得了组项。
                                   //没有父项，说明选择的是组。
            QString groupName = item->text(0);  //获得了组名。

            StationGroup *group;
            //查找组。
            for (int i = 0; i < currentList->count(); i++)
                if (currentList->getGroup(i)->getName() == groupName)
                    group = currentList->getGroup(i);
            //添加频道。
            group->addStation(station);
        }
    }

    refreshList();    //刷新显示。
}

void listDialog::delGroupOrStation()
{
    //获取当前项。
    QTreeWidgetItem *item = ui->treeWidget->currentItem();

    if (item)    //当前项存在，在进行删除操作。
    {
        if (item->parent())    //该项是一个频道，因为有父项（组）。
        {
            //如果该频道所在的组是第一个组，也就是默认组，删除操作不成功。
            if (item->parent()->text(0) == currentList->getGroup(0)->getName())
                QMessageBox::warning(this, "注意", "预置组不能进行删除操作！", QMessageBox::Ok);
            else    //可以删除。
            {
                StationGroup *group;
                QString groupName = item->parent()->text(0);                //获得组名。
                for (int i = 0; i < currentList->count(); i++)
                    if (currentList->getGroup(i)->getName() == groupName)
                        group = currentList->getGroup(i);                   //找到指定组。
                for (int j = 0; j < group->count(); j++)
                    if (group->getStation(j)->getName() == item->text(0))   //找到指定频道。
                        group->delStation(j);                               //删除频道。

                refreshList();    //更新显示。
            }
        }
        else                   //该项是一个组。
        {
            //如果这个组是第一个组，删除操作不成功。
            if (item->text(0) == currentList->getGroup(0)->getName())
                QMessageBox::warning(this, "注意", "预置组不能进行删除操作！", QMessageBox::Ok);
            else    //可以删除。
            {
                QString groupName = item->text(0);                //获得组名。
                for (int i = 0; i < currentList->count(); i++)
                    if (currentList->getGroup(i)->getName() == groupName)
                        currentList->delGroup(i);                 //删除指定组。

                refreshList();     //更新显示。
            }
        }
    }
}
