#include "demuxthread.h"

DemuxThread::DemuxThread(AVPacketQueue *audio_queue,AVPacketQueue *video_queue)
    :maudioQueue(audio_queue),mvideoQueue(video_queue)
{

}
int DemuxThread::open(const char *url)
{
    if(!url) return -1;

    if(!maudioQueue || !mvideoQueue) return -2;

    murl = url;
    mfmtCtx = avformat_alloc_context();
    int ret = avformat_open_input(&mfmtCtx, murl.c_str(), NULL, NULL);
    if(ret < 0)
    {
        av_strerror(ret, errorStr, sizeof(errorStr));
        printf("%s(%d) avformat_open_input failed:%d, %s\n", __FUNCTION__, __LINE__, ret, errorStr);
        return -3;
    }

    ret = avformat_find_stream_info(mfmtCtx, NULL);
    if(ret < 0)
    {
        av_strerror(ret, errorStr, sizeof(errorStr));
        printf("%s(%d) avformat_find_stream_info failed:%d, %s\n", __FUNCTION__, __LINE__, ret, errorStr);
        return -4;
    }

    av_dump_format(mfmtCtx, 0, murl.c_str(), 0);

    maudioStreamId = av_find_best_stream(mfmtCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    mvideoStreamId  = av_find_best_stream(mfmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    printf("%s(%d) audio_stream_:%d, video_stream_:%d\n", __FUNCTION__, __LINE__, maudioStreamId, mvideoStreamId);
    if(maudioStreamId < 0 || mvideoStreamId )
    {
        printf("no audio  or no video\n");
        return -5;
    }

    return 0;
}
void DemuxThread::stop()
{
    isrun=0;
}
void DemuxThread::run()
{
    printf("DemuxThread::Run() into\n");
    AVPacket packet;
    int ret = 0;
    while(isrun) {
        if(maudioQueue->size() > 100 || mvideoQueue->size() > 100)
        {
            QThread::msleep(10);
            printf("audio pkt size = %d video pkt size = %d\n",maudioQueue->size(),mvideoQueue->size());
            continue;
        }
        ret = av_read_frame(mfmtCtx, &packet);
        if(ret < 0)
        {
            av_strerror(ret, errorStr, sizeof(errorStr));
            printf("%s(%d) av_read_frame failed:%d, %s\n", __FUNCTION__, __LINE__, ret, errorStr);
            break;
        }
        if(packet.stream_index == maudioStreamId)
        {  // 音频包队列
            maudioQueue -> push(&packet);
            //printf("audio pkt size:%d\n", maudioQueue->size());
        } else if(packet.stream_index == mvideoStreamId)
        {  // 视频包队列
            mvideoQueue-> push(&packet);
            //printf("video pkt size:%d\n", mvideoQueue->size());
        } else
        {
            av_packet_unref(&packet);
        }
    }
    avformat_close_input(&mfmtCtx);
    printf("DemuxThread::Run() leave\n");

}

