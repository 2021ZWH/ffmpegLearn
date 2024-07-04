#ifndef AUDIOPLAY_H
#define AUDIOPLAY_H


#include <QAudioOutput>
#include <QAudioFormat>
#include <fstream>
#include <QThread>
#include "avframequeue.h"
#include "avsynctimer.h"
#ifdef __cplusplus
extern "C"{
#include"libavcodec/avcodec.h"
#include"libswresample/swresample.h"
#include"libswscale/swscale.h"
#include"libavutil/opt.h"
}
#endif
class AudioPlay:public QThread
{
    Q_OBJECT
public:
    AudioPlay(QObject *parent=NULL,AVFrameQueue *audio_queue = NULL,AVSyncTimer *avsyn = NULL);
    ~AudioPlay();
    void init(const AVChannelLayout &ch_layout,const int &sample_rate,AVSampleFormat format,AVRational time_base);

    void run() override;

private:
    AVFrameQueue *maudioFraQue = NULL;
    AVSyncTimer *mavsyn;

    QAudioOutput *maudioOut = NULL;
    QIODevice *mstreamOut = NULL;

    AVChannelLayout msrcChLayout;
    int msrcSampleRate;
    AVSampleFormat msrcFormat;

    AVChannelLayout mdstChLayout;
    int mdstSampleRate;
    AVSampleFormat mdstFormat;

    SwrContext *mswrCtx = NULL;
    uint8_t *moutBuff=NULL;

    AVRational mtimeBase;

  // std::ofstream outfile;

};

#endif // AUDIOPLAY_H
