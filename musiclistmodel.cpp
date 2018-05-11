#include "musiclistmodel.h"
#include<QStringList>
#include<QDebug>
MusicListModel::MusicListModel(QObject *parent) : QAbstractListModel(parent)
{

}

QModelIndex MusicListModel::addMusicItem(MusicInfoData *music)
{
  const int rowCnt=rowCount(QModelIndex());
  beginInsertColumns(QModelIndex(),rowCnt,rowCnt);
  m_pMusicList<<music;
  endInsertColumns();;
  return createIndex(rowCnt,0);
}

QModelIndex MusicListModel::insertMusic(MusicInfoData *music, int row)
{
    if(row >= rowCount(QModelIndex())){
        return addMusicItem(music);
    }else{
        beginInsertRows(QModelIndex(), row, row);
        m_pMusicList.insert(row, music);
        endInsertRows();
    }

    return createIndex(row,0);
}

MusicInfoData *MusicListModel::getNote(const QModelIndex &index)
{
    if(index.isValid()){
        return m_pMusicList.at(index.row());
    }else{
        return Q_NULLPTR;
    }
}

MusicListModel *MusicListModel::mapToListViewDelegrate(MusicListModel *musicListModel)
{
   //获取所有音乐的title信息，并分别统计首字母的个数
    QStringList titleList;
    int rowCnt=musicListModel->rowCount(QModelIndex());
    for(int i=0;i<rowCnt;i++)
    {
        titleList.append((musicListModel->getNote(musicListModel->index(i)))->title());
    }
    titleList.sort(Qt::CaseInsensitive);
    QMap<QString,int> map;
    QStringList::const_iterator constIterator;
    for (constIterator = titleList.constBegin(); constIterator != titleList.constEnd();++constIterator)
    {
        QString str=(*constIterator).toLocal8Bit().constData();
        QChar firstChar=str.at(0).toUpper();
        if(!map.contains(firstChar))
          {
            map.insert(firstChar,1);
          }
        else{
            map[firstChar]=map.value(firstChar)+1;
        }
    }
    MusicInfoData *music;
    MusicListModel *musicModel=musicListModel;
    static int row=0;
    for(int i=0;i<map.size();i++){
        QString key=map.keys().at(i);
        music=new MusicInfoData();
        music->setTitle(key.toUpper());
        musicModel->insertMusic(music,row);    
        row=row+map.value(key)+1;
        map[key]=row-map.value(key)-1;
    }
   rowCnt=musicModel->rowCount(QModelIndex());
   for(int i=0;i<rowCnt;i++)
  {
       QString str=static_cast<MusicInfoData *>(m_pMusicList.at(i))->title();
       qreal musicSize=static_cast<MusicInfoData *>(m_pMusicList.at(i))->musicSize();
       if(musicSize!=0.0){
          m_pMusicList.at(i)->setParentRow(map[str.at(0).toUpper()]);
       }else{
            m_pMusicList.at(i)->setParentRow(0);
       }
   }
    return musicModel;
}

void MusicListModel::addListMusicItem(QList<MusicInfoData *> musicList)
{
    int start = rowCount(QModelIndex());
    int end = start + musicList.count()-1;
    beginInsertRows(QModelIndex(), start, end);
    m_pMusicList << musicList;
    endInsertRows();
}


int MusicListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_pMusicList.count();
}

QVariant MusicListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_pMusicList.count())
        return QVariant();
    MusicInfoData* music=m_pMusicList[index.row()];
    if(role == title){
        return music->title();
    }else if(role==artist){
       return music->artistName();
    }else if(role==duration){
        return music->duration();
    }else if(role==album){
        return music->albumName();
    }else if(role==size){
        return music->musicSize();
    }else if(role==parentRow){
        return music->parentRow();
    }
    return QVariant();

}

bool MusicListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;
    MusicInfoData* music=m_pMusicList[index.row()];
    if(role == title){
        music->setTitle(value.toString());
    }else if(role==artist){
       music->setArtistName(value.toString());
    }else if(role==duration){
        music->setDuration(value.toTime());
    }else if(role==album){
        music->setAlbumName(value.toString());
    }else if(role==size){
        music->setMusicSize(value.toFloat());
    }else if(role==parentRow){
        music->setParentRow(value.toInt());
    }
    else{
        return false;
    }
    emit dataChanged(this->index(index.row()),
                     this->index(index.row()),
                    QVector<int>(1,role) );
    return true;
}

Qt::ItemFlags MusicListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsEditable ;
}



void MusicListModel::sort(int column, Qt::SortOrder order)
{
    Q_UNUSED(column)
    Q_UNUSED(order)
    std::stable_sort(m_pMusicList.begin(), m_pMusicList.end(), [](MusicInfoData* lhs, MusicInfoData* rhs){
        return lhs->title() < rhs->title();
    });

    emit dataChanged(index(0), index(rowCount(QModelIndex())-1));
}
