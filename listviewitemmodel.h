#ifndef LISTVIEWITEMMODEL_H
#define LISTVIEWITEMMODEL_H

#include <QAbstractListModel>
#include"listviewitemdate.h"
class ListViewItemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ListViewItemModel(QObject *parent = nullptr);
    ~ListViewItemModel();
    enum ItemRoles{
        ItemIcon,
        ItemContent
    };
    QModelIndex addItem(ListViewItemDate* note);
    ListViewItemDate* getNote(const QModelIndex& index);

signals:

public slots:

private:
    QList<ListViewItemDate *> m_pItemsList;
    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
};
#endif // LISTVIEWITEMMODEL_H
