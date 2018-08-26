#ifndef STATIONGROUP_H
#define STATIONGROUP_H

/*****************************************************
 * 频道组类（数据存储）
 * 主要成员
 * QString groupName        组名
 * QList<Station *> group   频道容器，存储当前组的频道指针
 * 方法
 * get/setName         获得/设置组名
 * load/toGroup        读取/写入组，
 *                     用于初始化/保存组，不接触文件，
 *                     使用QStringList与StationList交流
 * add/del/getStation  增加/删除/获取频道
 * count               获得该组内频道数量
 * ***************************************************/

#include "station.h"

class StationGroup
{
public:
    StationGroup();

    //存取分组名。
    QString getName();
    void setName(QString);

    //文件读取写入。
    void loadGroup(QStringList item);
    QStringList toGroup();

    //下级操作：增加、删除、获取电台。
    void addStation(Station *);
    void delStation(int);
    Station *getStation(int);
    int count();

private:
    QString groupName;
    QList<Station *> group;
};

#endif // STATIONGROUP_H
