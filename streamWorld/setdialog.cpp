#include "setdialog.h"
#include "ui_setdialog.h"

setDialog::setDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setDialog)
{
    ui->setupUi(this);

    //设置字体。
    QFont font;
    font.setFamily("FZCuHuoYi-M25S");
    font.setPointSize(11);
    this->setFont(font);

    //对外提供八个单选按钮的接口。
    radio = ui->radioButton;
    video = ui->videoButton;
    normal = ui->normalButton;
    big = ui->bigButton;
    task = ui->taskButton;
    tray = ui->trayButton;
    yes = ui->yesButton;
    no = ui->noButton;
    lineEdit = ui->lineEdit;

    //如果配置文件不存在，生成默认配置文件。
    TextIO file;
    if (!file.isExist("settings.ini"))
    {
        //获得默认配置。
        TextIO source;
        source.openFile("://settings.ini", readOnly);
        QStringList sourceList = source.outBlock();

        //写入配置文件。
        file.openFile("settings.ini", writeOnly);
        file.inBlock(sourceList);
        file.closeFile();
    }

    //读取配置文件并显示到设置窗口。
    QSettings *settings = new QSettings("settings.ini", QSettings::IniFormat);
    if (settings->value("ini/radio").toBool())
        this->radio->setChecked(true);
    else
        this->video->setChecked(true);

    if (settings->value("ini/normal").toBool())
        this->normal->setChecked(true);
    else
        this->big->setChecked(true);

    if (settings->value("ini/task").toBool())
        this->task->setChecked(true);
    else
        this->tray->setChecked(true);

    if (settings->value("ini/run").toBool())
        this->yes->setChecked(true);
    else
        this->no->setChecked(true);

    lineEdit->setText(settings->value("ini/cache").toString());
    delete settings;

    //开机启动的设置。
    QString reg = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    QSettings *setReg = new QSettings(reg, QSettings::NativeFormat);

    if (yes->isChecked())    //开机启动。
    {
        setReg->setValue(QApplication::applicationName(),
                         QDir::toNativeSeparators(QApplication::applicationFilePath()));
    }
    else
        setReg->remove(QApplication::applicationName());
    delete setReg;

    connect(ui->button, SIGNAL(clicked(bool)), this, SLOT(hide()));
}

setDialog::~setDialog()
{
    delete ui;
}

void setDialog::saveSettings()
{
    QSettings *settings = new QSettings("settings.ini", QSettings::IniFormat);
    settings->setValue("/ini/radio", this->radio->isChecked());
    settings->setValue("ini/normal", this->normal->isChecked());
    settings->setValue("ini/task", this->task->isChecked());
    settings->setValue("ini/run", this->yes->isChecked());
    settings->setValue("ini/cache", this->lineEdit->text());
    delete settings;
}

void setDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}
