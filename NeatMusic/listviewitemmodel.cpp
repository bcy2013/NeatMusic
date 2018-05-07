#include "listviewitemmodel.h"

ListViewItemModel::ListViewItemModel(QObject *parent) : QAbstractListModel(parent)
{

}

ListViewItemModel::~ListViewItemModel()
{

}

QModelIndex ListViewItemModel::addItem(ListViewItemDate *note)
{
    const int rowCnt =rowCount(QModelIndex());
    beginInsertRows(QModelIndex(), rowCnt, rowCnt);
    m_pItemsList << note;
    endInsertRows();
    return createIndex(rowCnt, 0);
}

ListViewItemDate *ListViewItemModel::getNote(const QModelIndex &index)
{
    if(index.isValid()){
        return m_pItemsList.at(index.row());
    }else{
        return Q_NULLPTR;
    }
}


QVariant ListViewItemModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_pItemsList.count())
        return QVariant();
    ListViewItemDate *data=m_pItemsList[index.row()];
    if(role==ItemIcon)
    {return data->getItemIcon();}
    else if(role==ItemContent)
       { return data->getItemContent();}
    return QVariant();
}

bool ListViewItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
     ListViewItemDate *data=m_pItemsList[index.row()];
     if(role==ItemIcon)
        {
         data->setItemIcon(value.value<QPixmap>());
         }
     else if(role==ItemContent)
        { data->setItemContent(value.toString());}
     else return false;
     emit dataChanged(this->index(index.row()),
                      this->index(index.row()),
                      QVector<int>(1,role));

     return true;
}


int ListViewItemModel::rowCount(const QModelIndex &parent) const
{    Q_UNUSED(parent)
     return m_pItemsList.count();
}
