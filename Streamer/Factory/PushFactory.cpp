#include "RtmpPush.h"
#include "DataToUI.h"
#include "PushFactory.h"

namespace Push
{
    PushFactory::PushFactory()
    {
    }

    PushFactory::~PushFactory()
    {
    }

    PushFlow* PushFactory::CreatePushClass(const std::string& type, const std::string& url)
    {
        if ("" == type)
        {
            DataToUI::GetInstance().setParament(QString("Can not initialize RtmpFFmpeg Lib."));
            return nullptr;
        }

        if ("RtmpPush" == type)
        {
            return new RtmpPush(url);
        }
    }
}
