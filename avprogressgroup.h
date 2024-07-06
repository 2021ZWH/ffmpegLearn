#ifndef AVPROGRESSGROUP_H
#define AVPROGRESSGROUP_H

#include <QWidget>
#include <QLabel>

#include "avprogressbar.h"
class AVProgressGroup : public QWidget
{
    Q_OBJECT
public:
    explicit AVProgressGroup(QWidget *parent = nullptr);
public slots:
    void setNowTime(int sec);
    void setAVDuration(int sec);
private:
    AVProgressBar *mprogressBar;
    QLabel *mnowTimeLabel;
    QLabel *mdurationLabel;
};

#endif // AVPROGRESSGROUP_H
