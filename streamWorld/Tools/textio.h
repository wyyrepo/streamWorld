#ifndef TEXTIO_H
#define TEXTIO_H

/*-------------------------------------------------------------
该类是以块/行的形式快速读取/修改文本文件的类，工具类与项目无关。
---------------------------------------------------------------
使用说明：
打开函数bool openFile(QString fileDir, int openModel)
返回值：打开成功/失败；参数1：文件路径，参数2：打开模式。
1.读取文件
textIO file; file.openFile("./1.txt", readonly);
--块形式读取： QStringList block = file.outblock();
--行形式读取： （获取总行数）int count = file.getLineAmount();
             （读取第二行）QString string = file.outLine(2);
2.写入文件
textIO file; file.openFile("./1.txt", writeonly);
--块形式写入： QStringList list; file.inBlock(List);
--行形式写入： QString string; file.inLine(string);
**注意**写入文件所有操作完毕后要关闭文件：file.closeFile();
        读取文件则不需要执行此操作。
-------------------------------------------------------------*/

#include <QFile>
#include <QTextStream>
#include <QStringList>

#define readOnly 0
#define writeOnly 1

class TextIO
{
public:
    TextIO();

    //查看文嘉是否存在的函数。
    bool isExist(QString);

    //打开文件的函数。
    bool openFile(QString fileDir, int openModel);

    //以块/行形式输出文本的函数。
    QStringList outBlock();
    QString outLine(int);

    //获取总行数的函数。
    int getLineAmount();

    //以块/行形式输入文本的函数。
    void inBlock(QStringList);
    void inLine(QString);

    //关闭文件的函数。
    void closeFile();

private:
    QFile *file;                //文件指针。
    QTextStream io;             //文本输入/输出流。
    int model;              //打开文件的模式，有readOnly，writeOnly两种。
    QStringList textList;       //存储块/行状文本。

    void dealFile();            //根据打开模式预处理文件。
};

#endif // TEXTIO_H
