#include "textio.h"

/*------------------------------------------
该类是以块/行的形式快速读取/修改文本文件的类。
------------------------------------------*/

//构造函数。
TextIO::TextIO()
{
    file = new QFile();
}

bool TextIO::isExist(QString dir)
{
    QFile file(dir);

    return file.exists();
}

//打开文件的函数，参数1是文件路径，参数2是打开模式（readonly, writeonly）。
bool TextIO::openFile(QString s, int m)
{
    //安全设置，先关闭文件指针。
    file->close();


    //设置文件路径。
    file->setFileName(s);
    if (!file->open(m==readOnly?QIODevice::ReadOnly:QIODevice::WriteOnly | QIODevice::Text))
    {
       file->close();
       return false;
    }

    //设置输入输出流。
    io.setDevice(file);

    //设置打开模式。
    model = m;

    //预处理函数。
    dealFile();

    return true;
}

//预处理函数。*
void TextIO::dealFile()
{
    if (model == readOnly)
    {
        //初始化存储空间。
        textList.clear();

        //行式读取文件。
        while(!io.atEnd())
        {
            textList.append(io.readLine());
        }

        //关闭文件指针。
        file->close();
    }
}

/*-----以块/行形式输出文本的函数。-----*/
QStringList TextIO::outBlock()    //块。
{
    return textList;
}

QString TextIO::outLine(int l)    //行。
{
    return textList.at(l-1);
}

//获得文本行数的函数。
int TextIO::getLineAmount()
{
    return textList.count();
}

//关闭文件的函数。
void TextIO::closeFile()
{
    file->close();
}

/*-----以块/行形式输入文本的函数。-----*/
void TextIO::inBlock(QStringList t)  //块。
{
    int i;

    for (i = 0; i < t.count(); i++)
        io<<t[i]<<endl;
}

void TextIO::inLine(QString s)       //行。
{
    io<<s<<endl;
}
