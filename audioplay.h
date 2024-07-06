#ifndef AUDIOPLAY_H
#define AUDIOPLAY_H


#include <QAudioOutput>
#include <QAudioFormat>
#include <QThread>
#include <QMutex>
#include "avframequeue.h"
#include "avsynctimer.h"
#include "playstate.h"
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


    void setState(PlayState state){
        QMutexLocker locker(&mmutex);
        mstate=state;
    }
signals:
    void timeChanged(int time);


private:
    void run() override;
    void inPlay();
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
    int nowtime=0;//现在播放的时间

    QMutex mmutex;

    PlayState mstate=PAUSE;

  // std::ofstream outfile;

};

#endif // AUDIOPLAY_H
