#include "playmusicshow.h"
#include "ui_playmusicshow.h"
PlayMusicShow::PlayMusicShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayMusicShow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    hide();

}

PlayMusicShow::~PlayMusicShow()
{
    delete ui;
}
void PlayMusicShow::toMax()
{   show();
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
