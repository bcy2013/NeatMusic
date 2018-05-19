#include "playlistdelegrate.h"

PlayListDelegrate::PlayListDelegrate(QObject *parent) : QStyledItemDelegate(parent)
  ,m_RowHeight(44)
{

}

QSize PlayListDelegrate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize result=QStyledItemDelegate::sizeHint(option,index);
    result.setHeight(m_RowHeight);
    return result;
}
