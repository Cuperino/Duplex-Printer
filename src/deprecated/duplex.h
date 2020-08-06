#ifndef DUPLEX_H
#define DUPLEX_H

#include <QDialog>

namespace Ui {
class Duplex;
}

class Duplex : public QDialog
{
    Q_OBJECT

public:
    explicit Duplex(QWidget *parent = 0);
    ~Duplex();
    void enableContinue( bool );

private slots:
    void on_pushButton_Ok_clicked();

private:
    Ui::Duplex *ui;
};

#endif // DUPLEX_H
