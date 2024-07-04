#include "audioplay.h"
#include <QDebug>

AudioPlay::AudioPlay(QObject *parent,AVFrameQueue *audio_queue,AVSyncTimer *avsyn )
    :QThread(parent),maudioFraQue(audio_queue),mavsyn(avsyn)
{
   //outfile.open("./1.pcm",std::ios::binary);

}
AudioPlay::~AudioPlay()
{
    if(moutBuff)
    delete [] moutBuff;
    if(mswrCtx)
    swr_free(&mswrCtx);


}
void AudioPlay::init(const AVChannelLayout &ch_layout,const int &sample_rate,AVSampleFormat format,AVRational time_base)
{
    mtimeBase=time_base;

    msrcChLayout=ch_layout;
    msrcSampleRate = sample_rate;
    msrcFormat=format;

    mdstFormat = AV_SAMPLE_FMT_S16;
    mdstSampleRate = sample_rate;
    av_channel_layout_default(&mdstChLayout,2);

    qDebug()<<"in"<<ch_layout.nb_channels<<" "<<sample_rate;
    QAudioFormat audioFmt;
    audioFmt.setByteOrder(QAudioFormat::LittleEndian);
    audioFmt.setSampleRate(mdstSampleRate);
    audioFmt.setSampleType(QAudioFormat::SignedInt);
    audioFmt.setSampleSize(16);
    audioFmt.setChannelCount(2);
    audioFmt.setCodec("audio/pcm");


    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if(!info.isFormatSupported(audioFmt))
    {
        audioFmt = info.nearestFormat(audioFmt);
        qDebug()<<"sample rate = "<<audioFmt.sampleRate();
        qDebug()<<"channel count"<<audioFmt.channelCount();
        qDebug()<<"sample size = "<<audioFmt.sampleSize();
        qDebug()<<"codec = "<<audioFmt.codec();
    }


    maudioOut = new QAudioOutput(audioFmt,this);

    maudioOut->setVolume(100);

    mstreamOut = maudioOut->start();
    mswrCtx = swr_alloc();

    swr_alloc_set_opts2(&mswrCtx,
                        &mdstChLayout,
                        mdstFormat,
                        mdstSampleRate,
                        &msrcChLayout,
                        msrcFormat,
                        msrcSampleRate,
                        0,
                        NULL);
    swr_init(mswrCtx);


    moutBuff = new uint8_t[409600];
    memset(moutBuff ,0,409600);

    mstreamOut = maudioOut->start();

}
void AudioPlay::run()
{
    if(maudioOut == NULL || mswrCtx == NULL )
    {
        qDebug()<<"init error";
        return ;

    }
    while(1)
    {
        AVFrame *frame=maudioFraQue->pop(2);

        if(frame == NULL) continue ;
        //if(moutBuff == NULL) return ;
        mavsyn->SetClock(frame->pts*av_q2d(mtimeBase));

       // if(strlen((char*)(*frame->extended_data)) == 0) continue ;
        //原采样样本 * 目的采样率除以原采样率 向上取整
        //根据原音频数据与目的音频数据的播放时长一致计算
        //播放时长 == 采样样品数/采样率
       // printf("frame sr = %d",frame->sample_rate);

        int dst_nb_samples =frame->nb_samples * mdstSampleRate / frame->sample_rate+256;
        /*参数 1：音频重采样的上下文
                参数 2：输出的指针。传递的输出的数组
                参数 3：输出的样本数量，不是字节数。单通道的样本数量。
                参数 4：输入的数组，AVFrame 解码出来的 DATA
                参数 5：输入的单通道的样本数量。
                */
        //重采样，返回重采样后的样品数量
        // if(av_sample_fmt_is_planar(msrcFormat) ) qDebug()<<"is planar";
        const uint8_t** in = (const uint8_t **)frame->extended_data;
        uint8_t **out = &moutBuff;
        int len = swr_convert(mswrCtx,
                              out,
                              dst_nb_samples,
                              in,
                              frame->nb_samples);

        if(len<=0)
        {
            return ;
        }
        //qDebug("convert length is: %d.\n",len);

        int out_size = av_samples_get_buffer_size(NULL,
                                                  mdstChLayout.nb_channels,
                                                  len,
                                                  mdstFormat,
                                                  0);
        //printf("buffer size = %d out sample = %d\n",out_size,len);


         //outfile.write((char *)moutBuff,out_size);
        //qDebug()<<out_size<<" "<<frame->sample_rate<<" "<<frame->nb_samples<<" "<<dst_nb_samples;

         while(maudioOut->bytesFree() < out_size)
             QThread::msleep(10);

        mstreamOut->write(QByteArray::fromRawData((char *)moutBuff,out_size));
        memset(moutBuff,0,out_size);





        av_frame_free(&frame);

    }



}
