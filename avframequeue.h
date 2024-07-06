#ifndef AVFRAMEQUEUE_H
#define AVFRAMEQUEUE_H

#include"blockingqueue.h"
#ifdef __cplusplus
extern "C"{
#include"libavcodec/avcodec.h"
}
#endif

class AVFrameQueue{
public:
    AVFrameQueue(){};
    ~AVFrameQueue()
    {
        this->release();
    }
    AVFrame *pop(const int timeout)
    {
        AVFrame *frame =NULL;

        int ret = mframeQueue.pop(frame,timeout);
        if(ret < 0)
        {
           // printf("queue pop");
        }

        return frame;
    }
    void push(AVFrame *frame)
    {
        AVFrame *_frame=av_frame_alloc();
        av_frame_move_ref(_frame,frame);
        mframeQueue.put(_frame);
    }
    int size() const
    {
        return mframeQueue.size();
    }
    void release()
    {
        while(mframeQueue.size())
        {
            AVFrame *frame=this->pop(2);
            if(frame) av_frame_free(&frame);
        }
    }
private:
    BlockingQueue<AVFrame*> mframeQueue;

};

#endif // AVFRAMEQUEUE_H
