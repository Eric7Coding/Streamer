#include "Logger.h"
#include "Address.h"
#include "ui_Address.h"
#include "DataBufferPointer.h"

Address::Address(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Address)
{
    setObjectName("Address Dialog");
    ui->setupUi(this);
    ui->IPText->setPlaceholderText("192.168.0.108");
    ui->PortText->setPlaceholderText("12678");

    qRegisterMetaType<AddressMsg>("AddressMsg");
    connect(this, &Address::send, this, &Address::close);
    connect(this, &Address::send, this, &Address::deleteLater);
}

Address::~Address()
{
    disconnect(this, &Address::send, this, &Address::close);
    disconnect(this, &Address::send, this, &Address::deleteLater);

    if (ui)
    {
        delete ui;
        ui = nullptr;
    }
}

void Address::on_OKbtn_clicked()
{
    Logger::LogMessage* log
        = new Logger::LogMessage
            (
                Logger::INFO,
                objectName().toStdString(),
                "Address Dialog got the IP address and port",
                QDateTime::currentDateTime().toString("yyyy-M-d h:m:s:zzz").toStdString()
            );
    DataBufferPointer::GetInstance().GetLog()->PushBack(std::shared_ptr<Logger::LogMessage>(log));

    emit send(
                AddressMsg
                    (
                        "" == ui->IPText->text()
                            ? ui->IPText->placeholderText()
                            : ui->IPText->text(),
                        "" == ui->PortText->text()
                            ? ui->PortText->placeholderText()
                            : ui->PortText->text()
                    )
             );
    hide();
}

void Address::on_Canbtn_clicked()
{
    exit(0);
}

void Address::on_IPText_returnPressed()
{
    ui->PortText->setFocus();
}

void Address::on_PortText_returnPressed()
{
    on_OKbtn_clicked();
}

void Address::keyReleaseEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        exit(0);
        break;
    default:
        QWidget::keyReleaseEvent(event);
        break;
    }
}
