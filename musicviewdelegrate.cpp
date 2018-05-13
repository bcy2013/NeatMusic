#include "musicviewdelegrate.h"
#include"listviewitemmodel.h"
#include<QPainter>

MusicViewDelegrate::MusicViewDelegrate(QObject *parent) : QStyledItemDelegate(parent)
  ,m_NormalColor("#f3f3f5")
  ,m_HoverColor("#e8e8eb")
  ,m_SelectColor("#dddde1")
  ,m_GlobleColor("#bc3f3e")
  ,m_iRowHeight(40)
  ,m_titleFont(QStringLiteral("Roboto"), 10, 60)
  ,m_titleSelectedFont(QStringLiteral("Roboto"), 10)
{

}

MusicViewDelegrate::~MusicViewDelegrate()
{

}


void MusicViewDelegrate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt=option;
    opt.rect.setWidth(193);
    paintBackground(painter,opt,index);
    paintContents(painter,opt,index);
}

void MusicViewDelegrate::setCurrentSelectedIndex(const QModelIndex &currentSelectedIndex)
{
    m_currentSelectedIndex=currentSelectedIndex;
}

void MusicViewDelegrate::setHoverIndex(const QModelIndex &currentHoverIndex)
{
    m_hoveredIndex=currentHoverIndex;
}

void MusicViewDelegrate::paintBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    if((option.state&QStyle::State_Selected)==QStyle::State_Selected)
    {
        QRect rect=option.rect;
        painter->fillRect(QRect(rect.x(),rect.y(),3,rect.height()),QColor(m_GlobleColor));
        painter->fillRect(QRect(rect.x()+3,rect.y(),rect.width()-3,rect.height()),QBrush(m_SelectColor));
    }
    else if((option.state&QStyle::State_MouseOver)==QStyle::State_MouseOver)
    {
        painter->fillRect(option.rect,QBrush(m_HoverColor));
    }
    else /*if((index.row() !=  m_currentSelectedIndex.row() - 1)
            && (index.row() !=  m_hoveredIndex.row() - 1))*/
    {
        painter->fillRect(option.rect,QBrush(m_NormalColor));
    }
}
#include<QDebug>
void MusicViewDelegrate::paintContents(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const int leftOffsetX = 14;
    const int topOffsetY = 15;   // space on top of title
    const int space = 15;       // space between title and date
    double rowPosX = option.rect.x();
    double rowPosY = option.rect.y();
    double rowWidth = option.rect.width();
    double rowHeight=option.rect.height();
    QString str=index.data(ListViewItemModel::ItemContent).toString();
    painter->setFont(QFont("微软雅黑", 11));
    QRectF textRectF(rowPosX+leftOffsetX+20+space,rowPosY,rowWidth,rowHeight);
    painter->drawText(textRectF,Qt::AlignVCenter,str);
    QRectF target(rowPosX+leftOffsetX, rowPosY+(rowHeight-20)*0.5, 20, 20);
    QRectF source(0.0, 0.0, 20.0, 20.0);
    painter->drawPixmap(target,index.data(ListViewItemModel::ItemIcon).value<QPixmap>(),source);
}

QSize MusicViewDelegrate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize result=QStyledItemDelegate::sizeHint(option, index);
    result.setHeight(m_iRowHeight);
    return result;
}
