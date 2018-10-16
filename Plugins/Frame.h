/*
 * 自定义存放H264裸数据的结构体，
 * 存到消息队列里面供推流对象以
 * 及生成本地文件的对象使用
 * data使用前必须为之开辟内存
 */

#ifndef _FRAME_H
#define _FRAME_H

#include <cstdint>

struct Frame
{
    int size;// The size of the raw data
    uint8_t* data;// The raw data of H264 format

    Frame() : size(0), data(nullptr) {}

    Frame(const int size) : size(size)
    {
        data = new uint8_t[this->size];
    }

    ~Frame()
    {
        if (data)
        {
            delete data;
            data = nullptr;
            size = 0;
        }
    }
};

#endif // _FRAME_H
