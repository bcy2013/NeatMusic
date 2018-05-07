#ifndef MUSICVIEWDELEGRATE_H
#define MUSICVIEWDELEGRATE_H

#include <QStyledItemDelegate>

class MusicViewDelegrate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MusicViewDelegrate(QObject *parent = nullptr);
    ~MusicViewDelegrate();
signals:

public slots:

    // QAbstractItemDelegate interface
public:
    enum States{
        Normal,
        Insert,
        Remove,
        MoveOut,
        MoveIn
    };
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setCurrentSelectedIndex(const QModelIndex &currentSelectedIndex);
    void setHoverIndex(const QModelIndex &currentHoverIndex);
private:
    void paintBackground(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index)const;
    void paintContents(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index)const;

    QColor m_NormalColor;
    QColor m_HoverColor;
    QColor m_SelectColor;
    QColor m_GlobleColor;

    QFont m_titleSelectedFont;
    QFont m_titleFont;

    QModelIndex m_currentSelectedIndex;
    QModelIndex m_hoveredIndex;

    int m_iRowHeight;

    // QAbstractItemDelegate interface
public:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
signals:
    void update(const QModelIndex &index);
};

#endif // MUSICVIEWDELEGRATE_H
