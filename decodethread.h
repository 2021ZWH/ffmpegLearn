#ifndef DECODETHREAD_H
#define DECODETHREAD_H

#include<QThread>
#include"avpacketqueue.h"
#include"avframequeue.h"
class DecodeThread:public QThread
{
public:
    DecodeThread(AVPacketQueue *packet_queue,AVFrameQueue *frame_queue);
    int init(const AVCodecParameters *par,const int &timeout);
    AVCodecContext *getCodecCtx(){
        return mcodecCtx;
    }
    void stop();

private:
    void run() override;
    AVCodecContext *mcodecCtx=NULL;
    AVPacketQueue *mpacketQueue=NULL;
    AVFrameQueue *mframeQueue=NULL;
    int mtimeout=0;
    char err2str[256];

    bool isrun=1;
};

#endif // DECODETHREAD_H
