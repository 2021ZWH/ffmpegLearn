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

    // mvideoPlay = new VideoPlay(this,mvideoFraQue);
    // ui->verticalLayout->addWidget(mvideoPlay);

    mframeCon =new FrameConvert(this,mvideoFraQue,mavsyn);

    mrender = new I420Render(this);
    ui->verticalLayout->addWidget(mrender);

    maudioPlay = new AudioPlay(this,maudioFraQue,mavsyn);

    connect(mframeCon,&FrameConvert::updateShow,mrender,&I420Render::updateShow);

}

Widget::~Widget()
{
    // 释放视频读取线程
    delete ui;
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

    AVCodecContext *codecCtx=maudioDecThread->getCodecCtx();
    maudioPlay->init(codecCtx->ch_layout,codecCtx->sample_rate,codecCtx->sample_fmt,mdemuxThread->getAudioTimebase());
    mframeCon->setTimeBase(mdemuxThread->getVideoTimebase());

    codecCtx = mvideoDecThread->getCodecCtx();
    mrender->init(codecCtx->width,codecCtx->height);


}

/**
 * @brief  视频播放/停止
 */
void Widget::on_but_open_clicked()
{

    if(ui->but_open->text() == QString::fromUtf8("开始播放"))
    {
        mavsyn->InitClock();
        mdemuxThread->start();
        mvideoDecThread->start();
        maudioDecThread->start();
        maudioPlay->start();
        mframeCon->start();
        ui->but_open->setText(QString::fromUtf8("停止播放"));

    }
    else
    {
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
        ui->but_pause->setText(QString::fromUtf8("继续"));
    }
    else
    {
        //->pause(false);
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
