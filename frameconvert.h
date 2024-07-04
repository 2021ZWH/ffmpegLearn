#ifndef FRAMECONVERT_H
#define FRAMECONVERT_H
#include<QThread>
#include"avframequeue.h"
#include"avsynctimer.h"
class FrameConvert:public QThread
{
    Q_OBJECT
public:
    FrameConvert(QObject *parent = nullptr,AVFrameQueue *video_queue = nullptr,AVSyncTimer *avsyn=nullptr);
    ~FrameConvert();
    void setTimeBase(AVRational time_base);
signals:
    void updateShow(unsigned char *buff);
private:
    void run() override;

    AVFrameQueue *mvideoFraQue = nullptr;
    AVSyncTimer *mavsyn = nullptr;

    AVRational mtimeBase;


};

#endif // FRAMECONVERT_H
