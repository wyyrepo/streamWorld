#ifndef LISTDIALOG_H
#define LISTDIALOG_H

/**********************************************************************
 * 列表窗口
 * 注1.包含三个StationList指针，(1)当前列表(2)音频列表(3)视频列表。音/视频窗口共用
 *     一个列表窗口，切换模式时，例如音频转视频，就执行当前列表=视频列表；代码中已经封
 *     装为changeModel(bool)方法。
 * 注2.存储列表的文件是radio.list和video.list。程序启动时从文件读取列表，退出时将列
 *     表写入文件(直接覆盖原文件)。如果读取时文件不存在，程序会自动生成一个简单的列表文
 *     件，以保证成功读取。
 * 注3.程序实现了对频道列表的添加/删除操作，还有修改和移动功能未实现。感兴趣的朋友可以
 *     自行实现。
 * 注4.程序启动时/修改列表后，根据列表内容即时生成频道的树型结构并更新界面。
 * 注5.实现上翻/下翻时从树型结构中获得地址比较复杂(涉及到父项子项)，因此单独设置了一个
 *     播放列表playList，出于安全考虑是循环的。上翻/下翻如果到头会从另一端开始。
 * 注6.该窗口不支持拖动，全程跟随音频/视频窗口移动，大致实现为：音频/视频窗口移动时发送
 *     新旧位置的差值给System，System根据该值移动列表窗口的位置。
 *********************************************************************/

#include <QDialog>
#include <QTreeWidgetItem>
#include <Tools/textio.h>
#include "Model/stationlist.h"
#include "widget/adddialog.h"

namespace Ui {
class listDialog;
}

class listDialog : public QDialog
{
    Q_OBJECT

public:
    explicit listDialog(QWidget *parent = 0);
    ~listDialog();

    void changeModel(bool videoModel = true);  //改变音视频模式的方法，默认改为视频模式。
    bool readList();                           //从文件中读取频道列表的方法。
    bool writeList();                          //将频道列表写入文件的方法。
    void refreshList();                        //转换音视频模式或者添加/删除频道等修改操作后，刷新列表显示的方法。

    char *getFirstAdd();                       //获得第一个电台的地址，用于程序初始化/切换音视频模式后装填空轨。
    QString getFirstName();                    //获得第一个电台的标题，用于程序初始化/切换音视频模式后设置标题。

    QString lastStation(char *);               //从播放列表中根据传入的频道取出上一个频道。
    QString nextStation(char *);               //从播放列表中根据传入的频道取出下一个频道。
    QString findName(char *address);           //根据地址寻找标题，用于上一曲等操作时设置音频标题。视频没有标题不需要。

private:
    Ui::listDialog *ui;

    QPoint record;              //记录参照点的对象。用于列表窗口跟随音/视频窗口移动。

    StationList *currentList;   //当前列表，有可能是视频列表，也有可能是音频列表。
    StationList *radioList;     //音频列表。
    StationList *videoList;     //视频列表。
    QStringList *playList;      //用于上一曲下一曲的播放列表，只保存播放地址，每次刷新界面时更新。
    void setPlayList();         //更新播放列表的函数。

    addDialog *addWidget;       //增加频道的弹出窗口。

signals:
    void listPlay(QString, QString);   //用户双击了频道列表后，向system发送电台标题和地址，要求播放。

private slots:
    void recordPos();          //这两个方法是用于跟随音/视频窗口移动的。
    void moveDif(QPoint);

    void selectStation(QTreeWidgetItem *, int); //用户双击频道列表后解析被选频道的槽，其中发送了listPlay()信号。

    void addGroupOrStation(QString, QString);   //增加频道的槽。
    void delGroupOrStation();                   //删除频道的槽。
};

#endif // LISTDIALOG_H
