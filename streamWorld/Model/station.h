#ifndef STATION_H
#define STATION_H

/**********************************
 * 频道类（数据存储）
 * 主要成员
 * QString name      频道标题
 * QString address   频道地址
 * *******************************/

#include <QStringList>

class Station
{
public:
    Station();
    Station(QString name, QString address);

    void setStation(QString station);
    void setName(QString name);
    void setAddress(QString address);

    QString getStation();
    QString getName();
    QString getAddress();

private:
    QString name;
    QString address;
};

#endif // STATION_H
