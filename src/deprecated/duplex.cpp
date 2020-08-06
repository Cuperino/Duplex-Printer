#include "duplex.h"
#include "ui_duplex.h"

Duplex::Duplex(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Duplex)
{
    ui->setupUi(this);
}

Duplex::~Duplex()
{
    delete ui;
}

void Duplex::enableContinue( bool ok )
{
    ui->pushButton_Ok->setEnabled( ok );
}

void Duplex::on_pushButton_Ok_clicked()
{
    this->close();
}
