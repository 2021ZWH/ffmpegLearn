#include "avprogressgroup.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QSpacerItem>
#include<QTime>
AVProgressGroup::AVProgressGroup(QWidget *parent)
    : QWidget{parent},mprogressBar (new AVProgressBar(this)),
    mnowTimeLabel(new QLabel(this)),mdurationLabel (new QLabel(this))
{
    mprogressBar->setOrientation(Qt::Orientations::enum_type::Horizontal);
    QVBoxLayout *vLayout=new QVBoxLayout(this);
    QHBoxLayout *hLayout=new QHBoxLayout();


    hLayout->addWidget(mnowTimeLabel,0);
    hLayout->addSpacerItem(new QSpacerItem(100,20,QSizePolicy::Expanding));
    hLayout->addWidget(mdurationLabel);

    vLayout->addWidget(mprogressBar);
    vLayout->addLayout(hLayout);

    vLayout->setSizeConstraint(QLayout::SetNoConstraint);//尽可能填满widget
    vLayout->setContentsMargins(0,0,0,0);//无边距

    mnowTimeLabel->setText("00:00:00");
    mdurationLabel->setText("00:00:00");

}
void AVProgressGroup::setNowTime(int sec)
{
    mprogressBar->setValue(sec);
    QTime nowtime(0,0,0,0);
    nowtime=nowtime.addSecs(sec);
    mnowTimeLabel->setText(nowtime.toString("hh:mm:ss"));
}

void AVProgressGroup::setAVDuration(int sec)
{
    mprogressBar->setMaximum(sec);
    QTime duration(0,0,0,0);
    duration=duration.addSecs(sec);
    mdurationLabel->setText(duration.toString("hh:mm:ss"));
}
