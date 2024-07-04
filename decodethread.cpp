#include "decodethread.h"

DecodeThread::DecodeThread(AVPacketQueue *packet_queue,AVFrameQueue *frame_queue)
    :mpacketQueue(packet_queue),mframeQueue(frame_queue)
{

}

int DecodeThread::init(const AVCodecParameters *par ,const int &timeout)
{
    if(!par) {
        printf("DecodeThread::Init par is NULL\n");
        return -1;
    }
    mtimeout=timeout;
    mcodecCtx = avcodec_alloc_context3(NULL);
    int ret = avcodec_parameters_to_context(mcodecCtx, par);
    //    avcodec_parameters_from_context // 合成复用的时候用
    if(ret < 0)
    {
        av_strerror(ret, err2str, sizeof(err2str));
        printf("avcodec_parameters_to_context failed, ret:%d, err2str:%s", ret, err2str);
        return -1;
    }
    const AVCodec *codec = avcodec_find_decoder(mcodecCtx->codec_id);
    if(!codec)
    {
        printf("avcodec_find_decoder failed\n");
        return -1;
    }
    ret = avcodec_open2(mcodecCtx, codec, NULL);
    if(ret < 0)
    {
        av_strerror(ret, err2str, sizeof(err2str));
        printf("avcodec_open2 failed, ret:%d, err2str:%s", ret, err2str);
        return -1;
    }
    printf("Init decode finish\n");
    return 0;
}

void DecodeThread::run()
{
    int ret = 0;
    AVFrame *frame = av_frame_alloc();
    while(1)
    {
        // 1920*1080*1.5*100
        if(mframeQueue->size() > 10)
        {
            QThread::msleep(10);
            continue;
        }
        // 从packet_queue读取packet
        AVPacket *packet = mpacketQueue->pop(mtimeout);
        if(packet)
        {
            // 送给解码器
            ret = avcodec_send_packet(mcodecCtx, packet);
            av_packet_free(&packet);
            if(ret < 0)
            {
                av_strerror(ret, err2str, sizeof(err2str));
                //printf("avcodec_send_packet failed, ret:%d, err2str:%s", ret, err2str);
                break;
            }
            // 从解码器读取frame
            while (true)
            {
                ret = avcodec_receive_frame(mcodecCtx, frame);  // 存在B帧的场景  B3-2  P2-3   I1-1 --> P3  B2  I1
                if(ret == 0)
                {
                    if(!frame->decode_error_flags)
                        mframeQueue->push(frame);
                    //printf("%s frame_queue size:%d\n ", mcodecCtx->codec->name, mframeQueue ->size());
                    continue;
                }
                else if(ret == AVERROR(EAGAIN))
                {
                    break;
                }
                else
                {
                    //abort_  = 1;
                    av_strerror(ret, err2str, sizeof(err2str));
                    printf("avcodec_receive_frame failed, ret:%d, err2str:%s", ret, err2str);
                    break;
                }
            }
            // 把frame发送给framequeue
        }
        else
        {
            // if(mtimeout == 3)
            // printf("audio no packet\n");
            // else
            //     printf("video no packet\n");
        }
    }
}
