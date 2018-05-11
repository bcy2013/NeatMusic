#include "listviewitemdate.h"

ListViewItemDate::ListViewItemDate(QObject *parent) : QObject(parent)
{

}

QPixmap ListViewItemDate::getItemIcon() const
{
   return m_ItemIcon;
}

void ListViewItemDate::setItemIcon(const QPixmap &itemIcon)
{
    m_ItemIcon=itemIcon;
}

QString ListViewItemDate::getItemContent() const
{
    return m_strItemContent;
}

void ListViewItemDate::setItemContent(const QString &itemContent)
{
    m_strItemContent=itemContent;
}





QDataStream &operator<<(QDataStream &stream, const ListViewItemDate *listData)
{
    return stream << listData->getItemIcon() << listData->getItemContent();
}

QDataStream &operator>>(QDataStream &stream, ListViewItemDate *&listData)
{
    listData = new ListViewItemDate();
    QPixmap pixmap;
    QString content;

    stream >> pixmap >> content ;
    listData->setItemIcon(pixmap);
    listData->setItemContent(content);
    return stream;
}
