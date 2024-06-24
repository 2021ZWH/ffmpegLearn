#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("Qt+ffmpeg视频播放（软解码 + OpenGL显示RGB）Demo"));


    // 使用QOpenGLWindow绘制
//     playImage = new PlayImage;
// #if USE_WINDOW
//     ui->verticalLayout->addWidget(QWidget::createWindowContainer(playImage));   // 这一步加载速度要比OpenGLWidget慢一点
// #else
//     ui->verticalLayout->addWidget(playImage);
// #endif

    playimagelabel = new PlayImageLabel;
    playimagelabel ->setParent(this);

    ui->verticalLayout->addWidget(playimagelabel);
    m_readThread = new ReadThread();
    //connect(m_readThread, &ReadThread::updateImage, playImage, &PlayImage::updateImage);
    connect(m_readThread,&ReadThread::updateImage,playimagelabel,&PlayImageLabel::updateImage);
    connect(m_readThread, &ReadThread::playState, this, &Widget::on_playState);
}

Widget::~Widget()
{
    // 释放视频读取线程
    if(m_readThread)
    {
        m_readThread->close();
        m_readThread->wait();
        delete m_readThread;
    }
    delete ui;
}

/**
 * @brief  获取本地视频路径
 */
void Widget::on_but_file_clicked()
{
    QString strName = QFileDialog::getOpenFileName(this,
                                                   QString::fromLocal8Bit("选择播放视频~！"),
                                                   "/",
                                                   QString::fromLocal8Bit("视频 (*.mp4 *.m4v *.mov *.avi *.flv);; 其它(*)"));
    if(strName.isEmpty())
    {
        return;
    }
    ui->com_url->setCurrentText(strName);
}

/**
 * @brief  视频播放/停止
 */
void Widget::on_but_open_clicked()
{

    if(ui->but_open->text() == QString::fromLocal8Bit("开始播放"))
    {
        m_readThread->open(ui->com_url->currentText());
        qDebug()<<"open "<<ui->com_url->currentText();
    }
    else
    {
        m_readThread->close();
    }
}

/**
 * @brief 视频暂停/继续
 */
void Widget::on_but_pause_clicked()
{
    if(ui->but_pause->text() == QString::fromLocal8Bit("暂停"))
    {
        m_readThread->pause(true);
        ui->but_pause->setText(QString::fromLocal8Bit("继续"));
    }
    else
    {
        m_readThread->pause(false);
        ui->but_pause->setText(QString::fromLocal8Bit("暂停"));
    }
}

/**
 * @brief        根据视频播放状态切换界面设置
 * @param state
 */
void Widget::on_playState(ReadThread::PlayState state)
{
    //qDebug()<<state;
    if(state == ReadThread::play)
    {
        this->setWindowTitle(QString::fromLocal8Bit("正在播放：%1").arg(m_readThread->url()));
        ui->but_open->setText(QString::fromLocal8Bit("停止播放"));
    }
    else
    {
        ui->but_open->setText(QString::fromLocal8Bit("开始播放"));
        ui->but_pause->setText(QString::fromLocal8Bit("暂停"));
        this->setWindowTitle(QString::fromLocal8Bit("Qt+ffmpeg视频播放（软解码 + OpenGL显示RGB）Demo  V%1"));
    }
}
