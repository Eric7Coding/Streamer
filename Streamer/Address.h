#ifndef _ADDRESS_H
#define _ADDRESS_H

#include "AddressMsg.h"
#include <QWidget>
#include <QKeyEvent>

namespace Ui
{
class Address;
}

class Address : public QWidget
{
    Q_OBJECT

public:
    explicit Address(QWidget* parent = 0);
    ~Address();

signals:
    void send(AddressMsg);

private slots:
    void on_OKbtn_clicked();
    void on_Canbtn_clicked();
    void on_IPText_returnPressed();
    void on_PortText_returnPressed();

private:
    void keyReleaseEvent(QKeyEvent* event);

private:
    Ui::Address *ui;
};

#endif // _ADDRESS_H
