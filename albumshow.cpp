#include "albumshow.h"
#include<QPainter>
#include<QBitmap>
#include<QGraphicsItemAnimation>
#include<QTimeLine>
#include<QGraphicsScene>
#include<QDir>
AlbumShow::AlbumShow(QObject *parent) : QObject(parent)
{
    QPixmap temp;
    temp.load(":/Resources/barbackground.png");
    albumBackGround=temp.scaled(320,320,Qt::KeepAspectRatio);
    QString path= QDir::currentPath()+"cover.jpg";
    temp.load(path);
    albumPicture=temp.scaled(220,220,Qt::KeepAspectRatio);
    angle=0.0;
    this->startTimer(20);

//    QGraphicsItemAnimation *animal=new QGraphicsItemAnimation;
//    animal->setItem(this);
//    QTimeLine *timeLine=new QTimeLine(4000);
//    timeLine->setLoopCount(0);
//    animal->setRotationAt(0.5,180);
//    //在动画执行完时图形项旋转360度
//    animal->setRotationAt(1,360);
//    //开始动画
//    timeLine->start();

}


QRectF AlbumShow::boundingRect() const
{
    qreal penWidth=2;
    return QRectF(-albumBackGround.width()/2-penWidth,-albumBackGround.height()-penWidth,albumBackGround.width()+penWidth*2,albumBackGround.height()+penWidth*2);
}

void AlbumShow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->drawPixmap(boundingRect().topLeft(),albumBackGround);

    QPointF point;
    point.setX(boundingRect().center().x()-albumPicture.width()/2);
    point.setY(boundingRect().center().y()-albumPicture.height()/2);
    QPixmap mask(":/Resources/barbackgroundr.png");
    albumPicture.setMask(mask.scaled(220,220,Qt::KeepAspectRatio).mask());
    painter->drawPixmap(point,albumPicture);
}

#include<QDebug>
static const double PI = 3.14159265358979323846264338327950288419717;
void AlbumShow::timerEvent(QTimerEvent *event)
{       Q_UNUSED(event);
        setTransformOriginPoint(boundingRect().center());
        angle+=1;
        setRotation(angle);

        if(angle>=360)
            angle=0;
        update();
}
