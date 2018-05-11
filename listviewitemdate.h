#ifndef LISTVIEWITEMDATE_H
#define LISTVIEWITEMDATE_H

#include <QObject>
#include<QPixmap>
class ListViewItemDate : public QObject
{
    Q_OBJECT
public:
    explicit ListViewItemDate(QObject *parent = nullptr);
    QPixmap getItemIcon()const;
    void setItemIcon(const QPixmap& itemIcon);

    QString getItemContent()const;
    void setItemContent(const QString& itemContent);




signals:

public slots:


private:
    QPixmap m_ItemIcon;
    QString m_strItemContent;
};
QDataStream &operator<<(QDataStream &stream, const ListViewItemDate* listData);
QDataStream &operator>>(QDataStream &stream, ListViewItemDate *&listData);
#endif // LISTVIEWITEMDATE_H
