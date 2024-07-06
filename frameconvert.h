#ifndef FRAMECONVERT_H
#define FRAMECONVERT_H
#include<QThread>
#include"avframequeue.h"
#include"avsynctimer.h"
#include"playstate.h"
class FrameConvert:public QThread
{
    Q_OBJECT
public:
    FrameConvert(QObject *parent = nullptr,AVFrameQueue *video_queue = nullptr,AVSyncTimer *avsyn=nullptr);
    ~FrameConvert();
    void setTimeBase(AVRational time_base);
    void setState(PlayState state)
    {
        mstate = state;
    }
signals:
    void updateShow(unsigned char *buff);
private:
    void run() override;
    void inPlay();

    AVFrameQueue *mvideoFraQue = nullptr;
    AVSyncTimer *mavsyn = nullptr;

    AVRational mtimeBase;

    PlayState mstate=PAUSE;


};

#endif // FRAMECONVERT_H
