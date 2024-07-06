#ifndef AVPACKETQUEUE_H
#define AVPACKETQUEUE_H

#include"blockingqueue.h"
#ifdef __cplusplus
extern "C" { // 大写的C
#include "libavcodec/avcodec.h"
}
#endif
class AVPacketQueue
{
public:
    AVPacketQueue(){};
    ~AVPacketQueue()
    {
        this->release();
    }
    int size() const
    {
        return mqueue.size();
    }
    AVPacket *pop(const int timeout)
    {
        AVPacket *packet = NULL;
        int ret=mqueue.pop(packet,timeout);

        if(ret < 0)
        {
            //printf("queue pop abort");
        }
        return packet;
    }
    void push(AVPacket *val)
    {
        AVPacket *mval = av_packet_alloc();
        av_packet_move_ref(mval,val);
        mqueue.put(mval);
    }
    void release()
    {
        while(mqueue.size())
        {
            AVPacket *packet = this->pop(2);
            if(packet) av_packet_free(&packet);
        }
    }

private:

    BlockingQueue<AVPacket *> mqueue;
};

#endif // AVPACKETQUEUE_H
