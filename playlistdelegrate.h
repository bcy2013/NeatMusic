#ifndef PLAYLISTDELEGRATE_H
#define PLAYLISTDELEGRATE_H

#include <QObject>
#include<QStyledItemDelegate>
class PlayListDelegrate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit PlayListDelegrate(QObject *parent = nullptr);

signals:

public slots:

private:
    int m_RowHeight;

    // QAbstractItemDelegate interface
public:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // PLAYLISTDELEGRATE_H
