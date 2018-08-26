#include "adddialog.h"
#include "ui_adddialog.h"

addDialog::addDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addDialog)
{
    ui->setupUi(this);

    //设置字体。
    QFont font;
    font.setFamily("FZCuHuoYi-M25S");
    font.setPointSize(11);
    this->setFont(font);

    connect(ui->groupButton, SIGNAL(pressed()), this, SLOT(stationInvalid()));
    connect(ui->stationButton, SIGNAL(pressed()), this, SLOT(groupInvalid()));
    connect(ui->yesButton, SIGNAL(clicked(bool)), this, SLOT(enter()));
    connect(ui->noButton, SIGNAL(clicked(bool)), this, SLOT(cancel()));
}

addDialog::~addDialog()
{
    delete ui;
}

void addDialog::stationInvalid()
{
    ui->combBox->setEnabled(false);

    ui->groupLine->setEnabled(true);
}

void addDialog::groupInvalid()
{
    ui->groupLine->setEnabled(false);

    ui->combBox->setEnabled(true);
}

void addDialog::enter()
{
    if (ui->groupButton->isChecked())    //用户选择添加组
    {
        QString groupName = ui->groupLine->text();
        if (!groupName.isEmpty())
        {
            emit addGroupOrStation(groupName, "");
            cancel();
        }
        else
            QMessageBox::warning(this, "注意", "请输入完整信息！", QMessageBox::Ok);
    }
    else                                 //用户选择添加频道
    {
        QString stationName = ui->titleLine->text();
        QString stationAddress = ui->addressLine->text();

        if (stationName.isEmpty() || stationAddress.isEmpty())
            QMessageBox::warning(this, "注意", "请输入完整信息！", QMessageBox::Ok);
        else
        {
            emit addGroupOrStation(stationName, stationAddress);
            cancel();
        }
    }
}

void addDialog::cancel()
{
    //添加电台区可用
    ui->stationButton->setChecked(true);
    ui->combBox->setEnabled(true);
    ui->titleLine->clear();
    ui->addressLine->clear();
    //添加组区不可用
    ui->groupLine->setEnabled(false);
    ui->groupLine->clear();

    this->close();
}
