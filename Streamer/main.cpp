#include "Address.h"
#include "Streamer.h"

#include <iostream>
#include <QMessageBox>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Address* addr = new Address;
    addr->show();
    QApplication::processEvents();
    int result = 0;
    Streamer* s = new Streamer;
    if (!s)
    {
        QMessageBox(QMessageBox::Critical, "Error", "Fail to initial UI!", QMessageBox::Ok).exec();
        return EXIT_FAILURE;
    }
    QApplication::processEvents();
    a.connect(addr, &Address::send, s, &Streamer::addr_receive);

    try
    {
        std::cout << "Before exec" << std::endl;
        result = a.exec();
        std::cout << "After exec" << std::endl;
    }
    catch (...)
    {
        QMessageBox::warning(0, "System Crashed!", "Hum，it seems that the system is crashed, sorry!");
    }

    if (s)
    {
        delete s;
        s = nullptr;
    }

    return result;
}
