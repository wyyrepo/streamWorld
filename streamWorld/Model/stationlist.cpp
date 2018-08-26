#include "stationlist.h"

StationList::StationList()
{

}

bool StationList::fromFile(QString fileDir)
{
    TextIO file;
    if (file.openFile(fileDir, readOnly))
    {
        QStringList text = file.outBlock();
        QStringList temp;
        temp<<text[0];

        for (int i = 1; i < text.count(); i++)
        {
            if (text[i].split("#").at(0) == "groupName")
            {
                StationGroup *group = new StationGroup();
                group->loadGroup(temp);
                list.append(group);

                temp.clear();
            }

            temp<<text[i];
        }

        StationGroup *group = new StationGroup();
        group->loadGroup(temp);
        list.append(group);

        return true;
    }

    return false;
}

bool StationList::toFile(QString fileDir)
{
    QStringList out;

    for (int i = 0; i < list.count(); i++)
        out<<list[i]->toGroup();

    TextIO file;
    if (file.openFile(fileDir, writeOnly))
    {
        file.inBlock(out);
        file.closeFile();

        return true;
    }

    return false;
}

void StationList::addGroup(StationGroup *group)
{
    list.append(group);
}

void StationList::delGroup(int n)
{
    list.removeAt(n);
}

StationGroup *StationList::getGroup(int n)
{
    return list.at(n);
}

int StationList::count()
{
    return list.count();
}


