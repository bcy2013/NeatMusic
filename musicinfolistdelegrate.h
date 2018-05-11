#ifndef MUSICINFOLISTDELEGRATE_H
#define MUSICINFOLISTDELEGRATE_H

#include <QObject>
#include<QStyledItemDelegate>
#include<QVector>
class MusicInfoListDelegrate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MusicInfoListDelegrate(QObject *parent = nullptr);
    ~MusicInfoListDelegrate();
signals:
    void update(const QModelIndex &index);
public slots:
    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setCurrentSelectedIndex(const QModelIndex &currentSelectedIndex);
    void setHoveredIndex(const QModelIndex &hoveredIndex);
    void paintBackground(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index)const;
    void paintLabels(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    QColor m_NomalColor;
    QColor m_HoverColor;
    QColor m_SelectedColor;
    int m_iRowHeight;

    QModelIndex m_currentSelectedIndex;
    QModelIndex m_hoveredIndex;

};

#endif // MUSICINFOLISTDELEGRATE_H
