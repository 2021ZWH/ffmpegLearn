#ifndef DEMUXTHREAD_H
#define DEMUXTHREAD_H

#include <QThread>
#include"avpacketqueue.h"
#ifdef __cplusplus
extern "C"{
#include "libavutil/avutil.h"
#include "libavformat/avformat.h"
}
#endif

class DemuxThread :public QThread
{
public:
    DemuxThread(AVPacketQueue *audio_queue,AVPacketQueue *video_queue);
    int open(const char *url);
    AVCodecParameters* getVideoCodecPar()
    {
        if(mvideoStreamId != -1)
        {
            return mfmtCtx->streams[mvideoStreamId]->codecpar;
        }
        else
        {
            return NULL;
        }
    }
    AVCodecParameters* getAudioCodecPar()
    {
        if(maudioStreamId!= -1)
        {
            return mfmtCtx->streams[maudioStreamId]->codecpar;
        }
        else
        {
            return NULL;
        }
    }

    AVRational getVideoTimebase()
    {
        return mfmtCtx->streams[mvideoStreamId]->time_base;
    }
    AVRational getAudioTimebase()
    {
        return mfmtCtx->streams[maudioStreamId]->time_base;
    }
    long long  getAVDuration()
    {
        long long secs= -1;
        if(AV_NOPTS_VALUE != mfmtCtx->duration)
        {
            int64_t duration =mfmtCtx->duration + (mfmtCtx->duration <= INT64_MAX - 5000 ? 5000:0);
            secs= duration / AV_TIME_BASE;
        }
        return secs;
    }
    void stop();
private:
    void run() override;
    AVPacketQueue *maudioQueue;
    AVPacketQueue *mvideoQueue;
    std::string murl;
    char errorStr[256];
    AVFormatContext *mfmtCtx=NULL;
    int maudioStreamId=-1;
    int mvideoStreamId=-1;

    bool isrun=1;
};

#endif // DEMUXTHREAD_H
