#include "musicinfolistdelegrate.h"
#include"musiclistmodel.h"
#include<QPainter>
#include<QDebug>
MusicInfoListDelegrate::MusicInfoListDelegrate(QObject *parent) : QStyledItemDelegate(parent)
  ,m_NomalColor("#fafafc")
  ,m_HoverColor("#f4f4f6")
  ,m_SelectedColor("#dddde1")
  ,m_iRowHeight(44)
{

}

MusicInfoListDelegrate::~MusicInfoListDelegrate()
{

}
void MusicInfoListDelegrate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt=option;
    paintBackground(painter, opt, index);
    paintLabels(painter, opt, index);
}

QSize MusicInfoListDelegrate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize result=QStyledItemDelegate::sizeHint(option, index);
    result.setHeight(m_iRowHeight);
    return result;
}

void MusicInfoListDelegrate::setCurrentSelectedIndex(const QModelIndex &currentSelectedIndex)
{
    m_currentSelectedIndex=currentSelectedIndex;
}

void MusicInfoListDelegrate::setHoveredIndex(const QModelIndex &hoveredIndex)
{
    m_hoveredIndex=hoveredIndex;
}
void MusicInfoListDelegrate::paintBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if((option.state&QStyle::State_Selected)==QStyle::State_Selected)
    {
         painter->fillRect(option.rect,QBrush(m_SelectedColor));
    }
    else if((option.state&QStyle::State_MouseOver)==QStyle::State_MouseOver)
    {
        painter->fillRect(option.rect,QBrush(m_HoverColor));
    }
    else if((index.row() !=  m_currentSelectedIndex.row())
            && (index.row() !=  m_hoveredIndex.row())){
            if(index.data(MusicListModel::size)==0){
                painter->fillRect(option.rect,QBrush(m_NomalColor));
            }else
            if((index.row()-index.data(MusicListModel::parentRow).toInt())%2==1){
                painter->fillRect(option.rect,QBrush(m_HoverColor));
            }else{
                painter->fillRect(option.rect,QBrush(m_NomalColor));
            }
        }
}


void MusicInfoListDelegrate::paintLabels(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //double rowPosX = option.rect.x();
    //double rowPosY = option.rect.y();
    double rowWidth = option.rect.width();
    //double rowHeight=option.rect.height();

    int fontMetricsWidthOffSet=30;
    QRect rect=option.rect;
    QString str=index.data(MusicListModel::title).toString();
    painter->setFont(QFont("Microsoft YaHei UI", 8));
    QFontMetrics fontMetrics(painter->fontMetrics());

    qreal strWidth=rowWidth*0.4-12-fontMetricsWidthOffSet;
    str = fontMetrics.elidedText(str, Qt::ElideRight, strWidth);
    rect.setX(12);
    rect.setWidth(rowWidth*0.4-12);
    painter->drawText(rect,Qt::AlignVCenter,str);

    strWidth=rowWidth*0.15-fontMetricsWidthOffSet;
    rect.setX(rowWidth*0.4);
    rect.setWidth(rowWidth*0.15);
    str=index.data(MusicListModel::artist).toString();
    str = fontMetrics.elidedText(str, Qt::ElideRight, strWidth);
    painter->drawText(rect,Qt::AlignVCenter,str);

    strWidth=rowWidth*0.2-fontMetricsWidthOffSet;
    rect.setX(rowWidth*0.55);
    rect.setWidth(rowWidth*0.2);
    str=index.data(MusicListModel::album).toString();
    str = fontMetrics.elidedText(str, Qt::ElideRight, strWidth);
    painter->drawText(rect,Qt::AlignVCenter,str);

    rect.setX(rowWidth*0.75);
    rect.setWidth(rowWidth*0.1);
    str=index.data(MusicListModel::duration).toTime().toString("mm:ss");
    painter->drawText(rect,Qt::AlignVCenter,str);

    rect.setX(rowWidth*0.85);
    rect.setWidth(rowWidth*0.15);
    if(index.data(MusicListModel::size).toInt()){
        qreal temp=index.data(MusicListModel::size).toDouble();
        str=str.setNum(temp,'f',2)+"MB";
        painter->drawText(rect,Qt::AlignVCenter,str);
    }

}

