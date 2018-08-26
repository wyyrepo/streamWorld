#include "stationgroup.h"

StationGroup::StationGroup()
{

}

QString StationGroup::getName()
{
    return groupName;
}

void StationGroup::setName(QString name)
{
    this->groupName = name;
}

void StationGroup::loadGroup(QStringList item)
{
    for (int i = 0; i < item.count(); i++)
    {
        QString left = item[i].split("#").at(0);
        QString right = item[i].split("#").at(1);

        if (left == "groupName")
        {
            this->groupName = right;
            group.clear();
        }
        else
        {
            Station *station = new Station(left, right);
            group.append(station);
        }
    }
}

QStringList StationGroup::toGroup()
{
    QStringList stringList;

    QString title = "groupName#" + this->groupName;
    stringList.append(title);

    for (int i = 0; i < group.count(); i++)
    {
        QString string = group[i]->getStation();
        stringList.append(string);
    }

    return stringList;
}

void StationGroup::addStation(Station *station)
{
    group.append(station);
}

void StationGroup::delStation(int n)
{
    group.removeAt(n);
}

Station *StationGroup::getStation(int n)
{
    return group.at(n);
}

int StationGroup::count()
{
    return group.count();
}
