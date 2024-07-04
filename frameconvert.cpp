#include "frameconvert.h"

 FrameConvert:: FrameConvert(QObject *parent ,AVFrameQueue *video_queue,AVSyncTimer *avsyn)
    :QThread(parent),mvideoFraQue(video_queue),mavsyn(avsyn)
{

}

 FrameConvert::~ FrameConvert()
{

}
void FrameConvert::setTimeBase(AVRational time_base)
{
    mtimeBase=time_base;
}
void  FrameConvert::run()
{
    while(1)
    {
        AVFrame *frame=mvideoFraQue->pop(10);
        if(frame == NULL) continue;
        int width =frame->width;
        int height=frame->height;

        double pts =  frame->pts * av_q2d(mtimeBase);
        double diff= pts - mavsyn->GetClock();


        while(diff > 0)
        {
            QThread::msleep(std::min(10,(int)(diff*1000)));
            printf("diff == %.3f pts == %.3f\n",diff,pts);
            pts =  frame->pts * av_q2d(mtimeBase);
            diff= pts - mavsyn->GetClock();
        }
        unsigned char *moutBuff = new unsigned char[width*height*4];


        int bytes =0;
        //Y分量部分
        for(int i=0;i<height;i++){
            memcpy(moutBuff+bytes,frame->data[0]+frame->linesize[0]*i,width);
            bytes+=width;
        }

        int u=height>>1;
        //U分量部分
        for(int i=0;i<u;i++){
            memcpy(moutBuff+bytes,frame->data[1]+frame->linesize[1]*i,width/2);
            bytes+=width/2;
        }

        //V分量部分
        for(int i=0;i<u;i++){
            memcpy(moutBuff+bytes,frame->data[2]+frame->linesize[2]*i,width/2);
            bytes+=width/2;
        }


        emit updateShow(moutBuff);

        av_frame_free(&frame);
    }
}
