#include "Logger.h"
#include "Streamer.h"
#include "addressmsg.h"
#include "ui_Streamer.h"
#include "Factory/PushFactory.h"
#include "Factory/EncodeFactory.h"
#include "Factory/DisplayFactory.h"
#include "Factory/NetworkFactory.h"
#include "Factory/InterceptFactory.h"

#include <QPainter>
#include <QDateTime>
#include <QMessageBox>

Streamer::Streamer(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::Streamer)
    , DEFAULT_WIDTH(1280)
    , DEFAULT_HEIGHT(720)
{
    ui->setupUi(this);
    setFixedSize(this->width(), this->height());
    ui->StopButton->setEnabled(false);
    setObjectName("Streamer");
    Logger::Log::GetInstance().start();

    // Register self-defined data type for signal and slot
    qRegisterMetaType<QImage>("QImage");
    qRegisterMetaType<NetworkParament>("NetworkParament");
    connect(&DataToUI::GetInstance(), &DataToUI::sendError, this, &Streamer::HandleError, Qt::AutoConnection);
    connect(&DataToUI::GetInstance(), &DataToUI::sendJitter, this, &Streamer::ShowJitter, Qt::AutoConnection);
    connect(&DataToUI::GetInstance(), &DataToUI::sendImage, this, &Streamer::DisplayImage, Qt::AutoConnection);
}

Streamer::~Streamer()
{
    // Disconnect the binding of signal and slot when closing client
    disconnect(&DataToUI::GetInstance(), &DataToUI::sendError, this, &Streamer::HandleError);
    disconnect(&DataToUI::GetInstance(), &DataToUI::sendJitter, this, &Streamer::ShowJitter);
    disconnect(&DataToUI::GetInstance(), &DataToUI::sendImage, this, &Streamer::DisplayImage);

    Logger::Log::GetInstance().ShutDown();
    Logger::Log::GetInstance().quit();
    Logger::Log::GetInstance().wait();
    Logger::Log::GetInstance().deleteLater();

    if (ui)
    {
        delete ui;
        ui = nullptr;
    }
}

// Initialize all plugins(modules)
void Streamer::InitialPlugins(const AddressMsg& addrmsg)
{
    /*
     * UI class holds every pointer of each module
     * To initialize each module,
     * UI class use factory pattern to gain the exact pointer of each object for its module,
     * and start sub-thread to run every module
     */

    Logger::LogMessage* log
        = new Logger::LogMessage
            (
                Logger::INFO,
                objectName().toStdString(),
                "initial successfully",
                QDateTime::currentDateTime().toString("yyyy-M-d h:m:s:zzz").toStdString()
            );
    DataBufferPointer::GetInstance().GetLog()->PushBack(std::shared_ptr<Logger::LogMessage>(log));

    // Initialize screen shot module
    Catpure::InterceptFactory catpure_factory;
    _catpure = catpure_factory.CreateInterceptClass("GdiGrab", DEFAULT_WIDTH, DEFAULT_HEIGHT);
    if (_catpure->Initial())
    {
        _catpure->start();
    }

    // Initialize network quality module
    Network::NetworkFactory network_factory;
    _network = network_factory.CreateNetworkClass("Tcping", addrmsg.ip.toStdString(), addrmsg.port.toShort());
    if (_network->Initial())
    {
        _network->start();
    }

    // Initialize video displaying module
    Display::DisplayFactory display_factory;
    _display = display_factory.CreateDisplayClass("PicSendUI", DEFAULT_WIDTH, DEFAULT_HEIGHT, NULL);
    if (_display->Initial())
    {
        _display->start();
    }

    // Initialize video encoding module
    Encode::EncodeFactory encode_factory;
    _encode = encode_factory.CreateEncodeClass("AvEncode", DEFAULT_WIDTH, DEFAULT_HEIGHT);
    if (_encode->Initial())
    {
        _encode->start();
    }

    // Initialize video stream pushing module
    Push::PushFactory push_factory;
    _push = push_factory.CreatePushClass("RtmpPush", std::string("rtmp://") + addrmsg.ip.toStdString() + std::string(":") + addrmsg.port.toStdString() + std::string("/live/text"));
    _push->Initial();
}

// Repaint the image to show on the screen
void Streamer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    if (this->image.size().width() <= 0)
        return;

    // Let the image scale as the same size as the window for showing on the software
    QImage image = this->image.scaled(ui->VideoDisplay->size(), Qt::KeepAspectRatio);

    int x = ui->VideoDisplay->width() - image.width();
    int y = ui->VideoDisplay->height() - image.height();

    x /= 2;
    y /= 2;

    x += ui->VideoDisplay->x();
    y += ui->VideoDisplay->y();

    painter.drawImage(QPoint(x, y), image);// Draw the image
    QMainWindow::paintEvent(event);
}

// Override the close event
void Streamer::closeEvent(QCloseEvent* event)
{
    /*
     * When clicking the close button to close the client,
     * it would close sub-threads firsly and release sub-threads
     * and release resourses. Then close parent thread
     * and release its resourses.
     * To close sub-threads according to changing the bool variable in the abstract class.
     * When every sub-thread is running, it would judge the bool variable to start the loop
     * with the true value of the bool variable.
     * When the bool variable becomes false, the sub-thread will quit the loop and stop running,
     * releasing the resource of the thread.
     * Every sub-thread has to call the function of deleteLater, as the sub-thread might be at
     * loop when the bool variable becomes false. The process might crash if forcing to quit,
     * so it is neccessary to call the function of deleteLater.
     */

    hide();

    _network->ShutDown();
    _network->quit();
    _network->wait();
    _network->deleteLater();

    _catpure->ShutDown();
    _catpure->quit();
    _catpure->wait();
    _catpure->deleteLater();

    _display->ShutDown();
    _display->quit();
    _display->wait();
    _display->deleteLater();

    _encode->ShutDown();
    _encode->quit();
    _encode->wait();
    _encode->deleteLater();

    if (_push->isRunning())
    {
        _push->StopPush();
    }
    _push->quit();
    _push->wait();
    _push->deleteLater();

    QMainWindow::closeEvent(event);
}

// Exception Handling
void Streamer::HandleError(const QString& err)
{
    QMessageBox(QMessageBox::Warning, "Error", err, QMessageBox::Ok).exec();
}

// Show network quality paraments, like jitter and delay
void Streamer::ShowJitter(const NetworkParament& parament)
{
    ui->MinValue->setText(QString::number(parament._min) + QString(" ms"));
    ui->MaxValue->setText(QString::number(parament._max) + QString(" ms"));
    ui->DelayValue->setText(QString::number(parament._delay) + QString(" ms"));
    ui->LossValue->setText(QString::number(parament._loss) + " %");
    ui->JitterValue->setText(QString::number(parament._minus_jitter) + " to " + QString::number(parament._plus_jitter) + QString(" ms"));
}

// Gain the image to display
void Streamer::DisplayImage(const QImage& image)
{
    this->image = image;
    update();// Call the update function to execute the paintEvent function
}

void Streamer::addr_receive(const AddressMsg& addrmsg)
{
    InitialPlugins(addrmsg);
    show();
}

// Slot of Gdi method in menu bar
void Streamer::on_GdiGrabAction_triggered()
{
    if (ui->GdiGrabAction->isChecked())
    {
        ui->GdiGrabAction->setChecked(true);
    }
    else
    {
        ui->GdiGrabAction->setChecked(true);
    }
}

// Slot of Tcping method in menu bar
void Streamer::on_TcpingAction_triggered()
{
    if (ui->TcpingAction->isChecked())
    {
        ui->TcpingAction->setChecked(true);
    }
    else
    {
        ui->TcpingAction->setChecked(true);
    }
}

// Slot of PicSendUI method in menu bar
void Streamer::on_PicSendUIAction_triggered()
{
    if (ui->PicSendUIAction->isChecked())
    {
        ui->PicSendUIAction->setChecked(true);
    }
    else
    {
        ui->PicSendUIAction->setChecked(true);
        ui->SDLAction->setChecked(false);
    }
}

// Slot of SDL methond in menu bar
void Streamer::on_SDLAction_triggered()
{
    if (ui->SDLAction->isChecked())
    {
        ui->SDLAction->setChecked(true);
    }
    else
    {
        ui->SDLAction->setChecked(true);
        ui->PicSendUIAction->setChecked(false);
    }
}

// Slot of AvEncode method in menu bar
void Streamer::on_AvEncodeAction_triggered()
{
    if (ui->AvEncodeAction->isChecked())
    {
        ui->AvEncodeAction->setChecked(true);
    }
    else
    {
        ui->AvEncodeAction->setChecked(true);
    }
}

// Slot of RtmpFFmpeg method in menu bar
void Streamer::on_RtmpPushAction_triggered()
{
    if (ui->RtmpPushAction->isChecked())
    {
        ui->RtmpPushAction->setChecked(true);
    }
    else
    {
        ui->RtmpPushAction->setChecked(true);
    }
}

// Slot of button for starting push in menu bar
void Streamer::on_StartButton_clicked()
{
    _push->StartPush();
    _push->start();

    Logger::LogMessage* log
        = new Logger::LogMessage
            (
                Logger::INFO,
                "Push Module",
                "is starting to push",
                QDateTime::currentDateTime().toString("yyyy-M-d h:m:s:zzz").toStdString()
            );
    DataBufferPointer::GetInstance().GetLog()->PushBack(std::shared_ptr<Logger::LogMessage>(log));

    ui->StartButton->setEnabled(false);
    ui->StopButton->setEnabled(true);
}

// Slot of button for stopping push in menu bar
void Streamer::on_StopButton_clicked()
{
    _push->StopPush();
    /*
     * When clicking the stopping push button,
     * the running process would not stop immediately,
     * even though executing the exit function
     * It would not stop until all threads process done
     */
    _push->exit();
    _push->wait();

    Logger::LogMessage* log
        = new Logger::LogMessage
            (
                Logger::INFO,
                "Push Module",
                "is stoping to push",
                QDateTime::currentDateTime().toString("yyyy-M-d h:m:s:zzz").toStdString()
            );
    DataBufferPointer::GetInstance().GetLog()->PushBack(std::shared_ptr<Logger::LogMessage>(log));

    ui->StopButton->setEnabled(false);
    ui->StartButton->setEnabled(true);
}
