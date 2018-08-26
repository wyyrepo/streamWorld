#include "station.h"

Station::Station(QString name, QString address)
{
    this->name = name;
    this->address = address;
}

void Station::setStation(QString station)
{
    this->name = station.split("#").at(0);
    this->address = station.split("#").at(1);
}

void Station::setName(QString name)
{
    this->name = name;
}

void Station::setAddress(QString address)
{
    this->address = address;
}

QString Station::getStation()
{
    QString station = this->name + "#" + this->address;
    return station;
}

QString Station::getName()
{
    return name;
}

QString Station::getAddress()
{
    return address;
}
