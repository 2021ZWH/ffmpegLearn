#ifndef AVPROGRESSBAR_H
#define AVPROGRESSBAR_H
#include<QSlider>
class AVProgressBar:public QSlider
{
public:
    AVProgressBar(QWidget *parent = nullptr);
protected:
    //void mousePressEvent(QMouseEvent *event);

};

#endif // AVPROGRESSBAR_H
