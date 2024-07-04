#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include <QThreadPool>
#include <QTimer>
#include "PlayState.h"
#include "demuxthread.h"
#include "decodethread.h"
#include "avpacketqueue.h"
#include "avframequeue.h"
#include "audioplay.h"
#include "frameconvert.h"
#include "i420render.h"
#include "avsynctimer.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_but_file_clicked();

    void on_but_open_clicked();

    void on_but_pause_clicked();

    void on_playState(PlayState state);

private:
    Ui::Widget *ui;
    I420Render *mrender=nullptr;
    AudioPlay *maudioPlay = nullptr;

    AVPacketQueue *maudioPktQue = nullptr;
    AVPacketQueue *mvideoPktQue = nullptr;
    AVFrameQueue *maudioFraQue = nullptr;
    AVFrameQueue *mvideoFraQue = nullptr;


    DemuxThread *mdemuxThread = nullptr;
    DecodeThread *mvideoDecThread = nullptr;
    DecodeThread *maudioDecThread=nullptr;
    FrameConvert *mframeCon = nullptr;

    AVSyncTimer *mavsyn;



};
#endif // WIDGET_H
