#include "favouritedelegrate.h"
#include<QPainter>
#include<QStyledItemDelegate>
FavouriteDelegrate::FavouriteDelegrate(QObject *parent) : QSqlRelationalDelegate(parent)
  ,m_NomalColor("#fafafc")
  ,m_HoverColor("#f4f4f6")
  ,m_SelectedColor("#dddde1")
  ,m_iRowHeight(44)
  ,m_bIsPlaying(false)
{

}
#include<QDebug>

void FavouriteDelegrate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if((option.state&QStyle::State_Selected)==QStyle::State_Selected)
    {
         painter->fillRect(option.rect,QBrush(m_SelectedColor));
    }
    else if((option.state&QStyle::State_MouseOver)==QStyle::State_MouseOver)
    {
        painter->fillRect(option.rect,QBrush(m_HoverColor));
    }

    QStyleOptionViewItem opt=option;//这里的矩形框是单元格，和liseView不同
    double rowPosX = option.rect.x();
    double rowPosY = option.rect.y();
    double rowWidth = option.rect.width();
    //double rowHeight=option.rect.height();
    QRect rect=option.rect;
    QRect tempRect;
    const QAbstractItemModel *model = index.model();
    painter->setFont(QFont("Microsoft YaHei UI", 10));
    QFontMetrics fontMetrics(painter->fontMetrics());
    qreal strWidth;
    //int fontMetricsWidthOffSet=30;
    if(index.column()==0){
        rect.setWidth(47);
        if(!m_bIsPlaying){
            QString id=model->data(index,Qt::DisplayRole).toString();
            painter->drawText(rect,Qt::AlignCenter,id);
        }else{
            QPixmap icon(":/Resources/btnmv.png");
            tempRect.setX(rowPosX+20);
            tempRect.setY(rowPosY+18);
            tempRect.setWidth(16);
            tempRect.setHeight(16);
            painter->drawPixmap(tempRect,icon);
        }
    }
    else if(index.column()==1){
           rect.setWidth(307);
           rect.setX(rowPosX+30);
           strWidth=rowWidth*0.8-30;
           QString title=model->data(index,Qt::DisplayRole).toString();
           title = fontMetrics.elidedText(title, Qt::ElideRight, strWidth);
           painter->drawText(rect,Qt::AlignVCenter,title);
    }else {
          strWidth=rowWidth*0.8;
          QString str=model->data(index,Qt::DisplayRole).toString();
          str = fontMetrics.elidedText(str, Qt::ElideRight, strWidth);
          painter->drawText(rect,Qt::AlignVCenter,str);
    }
}

QSize FavouriteDelegrate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize result=QSqlRelationalDelegate::sizeHint(option, index);
    if(index.column()==1){
        result.setWidth(50);
    }
    result.setHeight(m_iRowHeight);
    return result;
}

//void FavouriteDelegrate::paintLabels(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{

//}
