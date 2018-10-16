/*
 * UI class with signal and slot,
 * overriding paint event and
 * close event, and initialation of
 * every module
 */

#ifndef _STREAMER_H
#define _STREAMER_H

#include <QMainWindow>

class PushFlow;
class VideoEncode;
class VideoDisplay;
class NetworkQuality;
class InterceptScreen;
struct AddressMsg;
struct NetworkParament;

namespace Ui
{
class Streamer;
}

class Streamer : public QMainWindow
{
    Q_OBJECT

public:
    explicit Streamer(QWidget* parent = 0);
    ~Streamer();

public slots:
    void addr_receive(const AddressMsg& addrmsg);

protected:
    /*
     * override the paint event
     * when a image sent from the video display module,
     * it will be called to repaint to display on the screen
     */
    void paintEvent(QPaintEvent* event);
    /*
     * override the close event
     * firstly close sub-thread
     * then release the sourse of sub-thread
     * finally close parent thread
     * in case the crash of running process
     */
    void closeEvent(QCloseEvent* event);

private:
    void InitialPlugins(const AddressMsg& addrmsg);// Initial Plugins

private slots:
    void HandleError(const QString& err);// Signal for error
    void ShowJitter(const NetworkParament& parament);// Signal for network quality paraments
    void DisplayImage(const QImage& image);// Gain the image to display

private slots:
    void on_GdiGrabAction_triggered();// Slot of Gdi method in menu bar
    void on_TcpingAction_triggered();// Slot of Tcping method in menu bar
    void on_PicSendUIAction_triggered();// Slot of PicSendUI method in menu bar
    void on_SDLAction_triggered();// Slot of SDL methond in menu bar
    void on_AvEncodeAction_triggered();// Slot of AvEncode method in menu bar
    void on_RtmpPushAction_triggered();// Slot of RtmpFFmpeg method in menu bar
    void on_StartButton_clicked();// Slot of button for starting push in menu bar
    void on_StopButton_clicked();// Slot of button for stopping push in menu bar

private:
    // Default size for width and height of a saved image after screen shot
    const int DEFAULT_WIDTH;
    const int DEFAULT_HEIGHT;

private:
    Ui::Streamer* ui;
    PushFlow* _push;// The pointer for the abstact class of push flow module
    VideoEncode* _encode;// The pointer for the abstact class of video encoding module
    VideoDisplay* _display;// The pointer for the abstact class of video displaying module
    NetworkQuality* _network;// The pointer for the abstact class of network quality computing module
    InterceptScreen* _catpure;// The pointer for the abstact class of screen shot module
    std::string _addr;
    QImage image;// Saving current image sourse data
};

#endif // _STREAMER_H
