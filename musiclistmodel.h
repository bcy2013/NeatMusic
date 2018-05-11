#ifndef MUSICLISTMODEL_H
#define MUSICLISTMODEL_H

#include <QAbstractListModel>
#include"musicinfodata.h"
class MusicListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MusicListModel(QObject *parent = nullptr);
    enum MusicInfoRoles{
        title,
        artist,
        duration,
        album,
        size,
        parentRow
    };
    QModelIndex addMusicItem(MusicInfoData* music);
    QModelIndex insertMusic(MusicInfoData* music, int row);
    MusicInfoData* getNote(const QModelIndex& index);
    MusicListModel* mapToListViewDelegrate(MusicListModel* musicListModel);
    void addListMusicItem(QList<MusicInfoData*> musicList);
signals:

public slots:

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
private:
    QList<MusicInfoData *> m_pMusicList;

    // QAbstractItemModel interface
public:
    void sort(int column, Qt::SortOrder order) Q_DECL_OVERRIDE;
};

#endif // MUSICLISTMODEL_H
