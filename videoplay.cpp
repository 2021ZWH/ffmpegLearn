#include "playimagelabel.h"
#include<QPainter>
#include<QDebug>

PlayImageLabel::PlayImageLabel() {}

void PlayImageLabel::updateImage(QImage image){

    //qDebug()<<image.size();
    if(image.isNull() || image.size().width() <= 0) return ;


    {
        QMutexLocker locker(&mMutex);
        mImage=image;
    }
    static int count=0;
    count++;
    qDebug()<<"update img "<<count;
    this->update();
}
void PlayImageLabel::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    painter.setBrush(Qt::black);
    painter.drawRect(0,0,this->width(),this->height()); //先画成黑色

    if (mImage.size().width() <= 0) return;

    //将图像按比例缩放成和窗口一样大小

    // {
    //     try {
    //         QMutexLocker locker(&mMutex);
    //         QImage  img = mImage.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //         // //画面旋转
    //         // if(m_nRotateDegree > 0)
    //         // {
    //         //     QTransform matrix;
    //         //     matrix.rotate(m_nRotateDegree);
    //         //     img = img.transformed(matrix, Qt::SmoothTransformation);
    //         // }

    //         int x = this->width() - img.width();
    //         int y = this->height() - img.height();

    //         x /= 2;
    //         y /= 2;

    //         painter.drawImage(QPoint(x,y),img); //画出图像
    //     } catch (...) {
    //         qDebug()<<"异常";
    //     }

    // }


}
