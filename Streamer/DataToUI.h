/*
 * 各个模块抛出的异常以及其他相关信息，
 * 都会由该对象转抛给界面层进行处理
 */

#ifndef _DATA_TO_UI_H
#define _DATA_TO_UI_H

#include "NetworkParament.h"

#include <QImage>
#include <QObject>

class DataToUI : public QObject
{
    Q_OBJECT

public:
    static DataToUI& GetInstance()
    {
        static DataToUI _instance;
        return _instance;
    }

    // Send exception message
    void setParament(const QString& err)
    {
        emit sendError(err);
    }

    // Send Image message
    void setParament(const QImage& image)
    {
        emit sendImage(image);
    }

    // Send netwrok quality
    void setParament(const NetworkParament& parament)
    {
        emit sendJitter(parament);
    }

signals:
    void sendImage(QImage);// Signal to send QImage
    void sendError(QString);// Signal to send execption
    void sendJitter(NetworkParament);// Signal to send network quality

private:
    DataToUI(const DataToUI&) = delete;
    DataToUI& operator=(const DataToUI&) = delete;
    DataToUI() {}
    ~DataToUI() {}
};

#endif // _DATA_TO_UI_H
