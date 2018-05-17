#ifndef FAVOURITEDELEGRATE_H
#define FAVOURITEDELEGRATE_H

#include <QObject>
#include<QSqlRelationalDelegate>
class FavouriteDelegrate : public QSqlRelationalDelegate
{
    Q_OBJECT
public:
    explicit FavouriteDelegrate(QObject *parent = nullptr);



signals:

public slots:

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    //void paintLabels(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    QColor m_NomalColor;
    QColor m_HoverColor;
    QColor m_SelectedColor;
    int m_iRowHeight;
    bool m_bIsPlaying;
};

#endif // FAVOURITEDELEGRATE_H
