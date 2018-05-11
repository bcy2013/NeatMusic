#include "toast.h"
#include<QPainter>
#include<QDebug>
Toast::Toast(QWidget *parent) : QWidget(parent)
  ,m_aniProperty(new QPropertyAnimation(this,"windowOpacity"))
  ,m_graphicsOpacity(new QGraphicsOpacityEffect)
  ,isHavePicture(false)
{
    m_pLabel=new QLabel(this);
    m_pPicture=new QLabel(this);
    m_pPicture->setVisible(false);
    QPixmap pic(":/Resources/person.png");
    m_pPicture->setFixedSize(30,30);
    m_pPicture->setPixmap(pic.scaled(m_pPicture->width(),m_pPicture->height(),Qt::IgnoreAspectRatio));
    QString ss = "QLabel{ "
                 "  background-color: transparent; "
                 "  color: white;"
                 "  font: 14pt  ; "
                 "font-family:Microsoft YaHei UI;"
                 "}";
    this->setStyleSheet(ss);
    QVBoxLayout *mainLayout=new QVBoxLayout(this);
    mainLayout->addWidget(m_pPicture,0,Qt::AlignCenter);
    mainLayout->addWidget(m_pLabel,0,Qt::AlignCenter);
    mainLayout->setMargin(0);
    m_graphicsOpacity->setOpacity(1);//没效果的
    //setGraphicsEffect(m_graphicsOpacity);
    setLayout(mainLayout);


    hide();
    //setFixedSize(280,50);
    setWindowFlags(Qt::FramelessWindowHint
                   |Qt::ToolTip
                   );
    this->setAttribute(Qt::WA_TranslucentBackground);
    connect(this,&Toast::modelChanged,[this](){
        m_pPicture->setVisible(true);
        update();
    });
}

void Toast::setText(const QString& text)
{
    m_pLabel->setText(text);
}

void Toast::setPicture(const bool &pic)
{
    isHavePicture=pic;
    if(isHavePicture)
        emit modelChanged();
}

bool Toast::picture() const
{
    return isHavePicture;
}

void Toast::pop()
{
    show();
    m_aniProperty->setDuration(2000);
    m_aniProperty->setStartValue(0);
    m_aniProperty->setKeyValueAt(0.3,1.0);
    m_aniProperty->setKeyValueAt(0.9,1.0);
    m_aniProperty->setEndValue(0);

    m_aniProperty->start();
    connect(m_aniProperty,&QPropertyAnimation::finished,[this](){
        hide();
        m_aniProperty->deleteLater();
        this->deleteLater();
    });
}




//qreal Toast::opacity() const
//{
//    return m_fOpacity;
//}

//void Toast::setOpacity(const qreal &opacity)
//{
//    m_fOpacity=opacity;
//}

void Toast::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    QPainterPath painterPath;
    painterPath.addRoundedRect(this->rect(),6,6);
    painter.fillPath(painterPath,QColor(53,53,53));


}
