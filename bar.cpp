#include "bar.h"
#include<QPainter>
#include<QGraphicsSceneMouseEvent>
Bar::Bar(QObject *parent) : QObject(parent)
{
    QPixmap temp(":/Resources/bar.png");
    barImg=temp.scaled(140,200,Qt::KeepAspectRatio);
    m_State=State::PauseState;
    QPointF orign;
    orign.setX(boundingRect().topLeft().x()+42);
    orign.setY(boundingRect().topLeft().y()+32);
        setTransformOriginPoint(orign);
        setRotation(-30);
   // setFlags(QGraphicsItem::ItemIsMovable);
}


QRectF Bar::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(0-barImg.width()/2-adjust,0-barImg.height()/2-adjust,
                    barImg.width()+adjust*2,barImg.height()+2*adjust);
}

void Bar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
     painter->setRenderHint(QPainter::Antialiasing, true);
    painter->drawPixmap(boundingRect().topLeft(),barImg);
}


void Bar::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
        {
            //如果不是鼠标左键按下，则忽略该事件
            event->ignore();
            return;
        }

        //如果是鼠标左键按下，改变光标形状
    QPointF orign;
    orign.setX(boundingRect().topLeft().x()+42);
    orign.setY(boundingRect().topLeft().y()+32);
        setTransformOriginPoint(orign);


        if(m_State==State::PauseState){
             setRotation(30);
             m_State=State::PlayState;
        }else if(m_State==State::PlayState){
            setRotation(-30);
            m_State=State::PauseState;
        }

}
