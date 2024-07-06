#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget),maudioPktQue(new AVPacketQueue),
    mvideoPktQue(new AVPacketQueue),maudioFraQue(new AVFrameQueue)
    ,mvideoFraQue(new AVFrameQueue),mdemuxThread(new DemuxThread(maudioPktQue,mvideoPktQue))
    ,mvideoDecThread(new DecodeThread(mvideoPktQue,mvideoFraQue))
    ,maudioDecThread(new DecodeThread(maudioPktQue,maudioFraQue))
    ,mavsyn (new AVSyncTimer)
{

    ui->setupUi(this);
    this->setWindowTitle(QString::fromUtf8("Qt+ffmpeg视频播放（软解码 + OpenGL显示RGB）Demo"));

    mframeCon =new FrameConvert(this,mvideoFraQue,mavsyn);
    maudioPlay = new AudioPlay(this,maudioFraQue,mavsyn);


    connect(mframeCon,&FrameConvert::updateShow,ui->yuvGLRender,&I420Render::updateShow);
    connect(maudioPlay,&AudioPlay::timeChanged,ui->progressGroup,&AVProgressGroup::setNowTime);

    mavsyn->InitClock();



}

Widget::~Widget()
{
    // 释放视频读取线程
    delete ui;

    delete maudioFraQue;
    delete mvideoFraQue;
}

void Widget::startAV()
{
    mstate=PlayState::PLAY;
    mframeCon->setState(mstate);
    maudioPlay->setState(mstate);
}
void Widget::stopAV()
{
    mstate=PlayState::PAUSE;
    mframeCon->setState(mstate);
    maudioPlay->setState(mstate);
}
void Widget::endAV()
{
    mstate=PlayState::END;
    mframeCon->setState(mstate);
    maudioPlay->setState(mstate);
}
/**
 * @brief  获取本地视频路径
 */
void Widget::on_but_file_clicked()
{
    QString strName = QFileDialog::getOpenFileName(this,
                                                   QString::fromUtf8("选择播放视频~！"),
                                                   "/",
                                                   QString::fromUtf8("视频 (*.mp4 *.m4v *.mov *.avi *.flv);; 其它(*)"));
    if(strName.isEmpty())
    {
        return;
    }

    ui->com_url->setCurrentText(strName);

    mdemuxThread->open(strName.toStdString().c_str());
    maudioDecThread->init(mdemuxThread->getAudioCodecPar(),2);
    mvideoDecThread->init(mdemuxThread->getVideoCodecPar(),2);

    ui->progressGroup->setAVDuration(mdemuxThread->getAVDuration());

    AVCodecContext *codecCtx=maudioDecThread->getCodecCtx();
    maudioPlay->init(codecCtx->ch_layout,codecCtx->sample_rate,codecCtx->sample_fmt,mdemuxThread->getAudioTimebase());
    mframeCon->setTimeBase(mdemuxThread->getVideoTimebase());

    codecCtx = mvideoDecThread->getCodecCtx();
    ui->yuvGLRender->init(codecCtx->width,codecCtx->height);

    mdemuxThread->start();
    mvideoDecThread->start();
    maudioDecThread->start();

    maudioPlay->start();
    mframeCon->start();



}

/**
 * @brief  视频播放/停止
 */
void Widget::on_but_open_clicked()
{

    if(ui->but_open->text() == QString::fromUtf8("开始播放"))
    {

        ui->but_open->setText(QString::fromUtf8("结束播放"));
        startAV();
    }
    else
    {
        ui->but_open->setText(QString::fromUtf8("开始播放"));

        endAV();
       // m_readThread->close();
    }
}

/**
 * @brief 视频暂停/继续
 */
void Widget::on_but_pause_clicked()
{
    if(ui->but_pause->text() == QString::fromUtf8("暂停"))
    {
       // m_readThread->pause(true);
        stopAV();
        ui->but_pause->setText(QString::fromUtf8("继续"));
    }
    else
    {
        //->pause(false);
        startAV();
        ui->but_pause->setText(QString::fromUtf8("暂停"));
    }
}

/**
 * @brief        根据视频播放状态切换界面设置
 * @param state
 */
void Widget::on_playState(PlayState state)
{
    //qDebug()<<state;
    if(state == PlayState::PLAY)
    {
        //this->setWindowTitle(QString::fromUtf8("正在播放：%1").arg(m_readThread->url()));
        ui->but_open->setText(QString::fromUtf8("停止播放"));
    }
    else
    {
        ui->but_open->setText(QString::fromUtf8("开始播放"));
        ui->but_pause->setText(QString::fromUtf8("暂停"));
        this->setWindowTitle(QString::fromUtf8("Qt+ffmpeg视频播放（软解码 + OpenGL显示RGB）Demo  V%1"));
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    endAV();
    maudioDecThread->stop();
    mvideoDecThread->stop();
    mdemuxThread->stop();

}
