#ifndef PLAYIMAGELABEL_H
#define PLAYIMAGELABEL_H
#include<QLabel>
#include<QPaintEvent>
#include<QImage>
#include<QMutex>
class PlayImageLabel:public QLabel
{
public:
    PlayImageLabel();
    void updateImage(QImage image);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QImage mImage;
    QMutex mMutex;
};

#endif // PLAYIMAGELABEL_H
