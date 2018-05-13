#include "playmusicshow.h"
#include"filter.h"
#include<QImage>
#include<QPainter>
#include<QGraphicsBlurEffect>
#include<QDebug>
#include<QResizeEvent>
#include"albumshow.h"
#include"bar.h"
#include<QGraphicsItemAnimation>
#include<QTimeLine>
#include<QGraphicsEllipseItem>
#include<QResizeEvent>
#include<QOpenGLWidget>
PlayMusicShow::PlayMusicShow(QWidget *parent) :
    QGraphicsView(parent)
{
    initBackground();
    initScene();
    //使用抗锯齿渲染
    this->setRenderHint(QPainter::Antialiasing);
        //设置缓冲背景，加速渲染
  this->setCacheMode(QGraphicsView::CacheBackground);
    QRect rect=parent->geometry();
    resize(rect.width(),rect.height()-60);
//   this->setScene(m_pGraScene);
//    QTimeLine *timeLine=new QTimeLine(4000*8);
//    timeLine->setFrameRange(0,100);
//    timeLine->setLoopCount(0);
//    QGraphicsItemAnimation *animal=new QGraphicsItemAnimation;
//    animal->setItem(albumShowItem);
//    animal->setTimeLine(timeLine);
//    animal->setRotationAt(0.5,180);
//    //在动画执行完时图形项旋转360度
//    animal->setRotationAt(1,360);
//    //开始动画
//     timeLine->start();
   // albumShowItem->setPos(164,134);

    //m_pGraScene->setSceneRect(-m_pGraScene->width()/2,-m_pGraScene->height()/2,m_pGraScene->width()/4,m_pGraScene->height()/4);
    //this->setBackgroundBrush(QBrush(gauss.scaled(parent->geometry().size(),Qt::IgnoreAspectRatio)));
    this->setAutoFillBackground(true);
    //this->setViewport(new QOpenGLWidget());
    this->setWindowFlags(Qt::FramelessWindowHint);
    hide();
}

PlayMusicShow::~PlayMusicShow()
{

}
void PlayMusicShow::toMax()
{
    show();
    QPropertyAnimation *m_aniPropertyGeometry=new QPropertyAnimation(this,"geometry");
    m_aniPropertyGeometry->setDuration(250);
    m_aniPropertyGeometry->setStartValue(QRect(0,geometry().height(),0,0));
    m_aniPropertyGeometry->setEndValue(geometry());
    m_aniPropertyGeometry->setEasingCurve(QEasingCurve::InOutQuad);
    m_aniPropertyGeometry->start(QAbstractAnimation::DeleteWhenStopped);
}

void PlayMusicShow::toMin()
{
    QPropertyAnimation *m_aniPropertyGeometry=new QPropertyAnimation(this,"geometry");
    m_aniPropertyGeometry->setDuration(250);
    m_aniPropertyGeometry->setStartValue(geometry());
    m_aniPropertyGeometry->setEndValue(QRect(0,geometry().height(),0,0));
    m_aniPropertyGeometry->setEasingCurve(QEasingCurve::InOutQuad);
    m_aniPropertyGeometry->start(QAbstractAnimation::DeleteWhenStopped);
    connect(m_aniPropertyGeometry,&QPropertyAnimation::finished,[this](){
        hide();
    });
}

void PlayMusicShow::initBackground()
{
    QImage  backgroundImg;
    backgroundImg.load(":/Resources/zhuanjismall.jpg");
    gauss=GaussFilter(backgroundImg, 100);
}

void PlayMusicShow::initScene()
{
    m_pGraScene=new QGraphicsScene();
    m_pGraScene->setBackgroundBrush(QBrush(Qt::NoBrush));
    AlbumShow *albumShowItem=new AlbumShow();
    Bar *bar=new Bar();
    QPoint tempPoint(0,0);//zuobiao hao nan
    albumShowItem->setPos(0,0);
    bar->setPos(QPoint(80,-300));
    m_pGraScene->addItem(albumShowItem);
    m_pGraScene->addItem(bar);
    QRect rect=this->geometry();
    int setX=-rect.width()/2;
    int setY=-rect.height()/2;
    m_pGraScene->setSceneRect(-200,-400,200,200);
    this->setScene(m_pGraScene);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void PlayMusicShow::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter,rect);
    //painter->drawImage(rect,gauss.scaled(QSize(rect.width(),rect.height()),Qt::IgnoreAction));
    painter->drawImage(rect,gauss.scaled(rect.width(),rect.height()));

}
