#ifndef STATIONLIST_H
#define STATIONLIST_H

/****************************************************
 * 频道列表类（数据存储）
 * 主要成员
 * QList<StationGroup *> list  组容器，存储组指针
 * 方法
 * from/toFile      从文件读取/写入列表，
 *                  用于初始化/保存列表，
 *                  内部使用QStringList操作StationGroup
 * add/del/getGroup 增加/删除/获取组
 * count            获取当前列表的组的数量
 ****************************************************/

#include "stationgroup.h"
#include "Tools/textio.h"

class StationList
{
public:
    StationList();

    //文件读取与写入。
    bool fromFile(QString);
    bool toFile(QString);

    //下级操作：增加、删除、获取组。
    void addGroup(StationGroup *);
    void delGroup(int);
    StationGroup *getGroup(int);
    int count();

private:
    QList<StationGroup *> list;
};

#endif // STATIONLIST_H
