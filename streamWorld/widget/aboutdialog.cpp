#include "aboutdialog.h"
#include "ui_aboutdialog.h"

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);

    //设置字体。
    QFont font;
    font.setFamily("FZCuHuoYi-M25S");
    font.setPointSize(11);
    this->setFont(font);

    font.setPointSize(15);
    ui->titleLabel->setFont(font);

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(hide()));
}

aboutDialog::~aboutDialog()
{
    delete ui;
}

void aboutDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}
