#ifndef MUSICDBMANAGER_H
#define MUSICDBMANAGER_H

#include <QObject>
#include<QSqlDatabase>
#include<QList>
#include"musicinfodata.h"
class MusicDbManager : public QObject
{
    Q_OBJECT
public:
    explicit MusicDbManager(const QString& path,const bool& doWhat,QObject *parent = nullptr);
    int MusicDbManager::getLastRowID();//ok
    bool addOneMusic(MusicInfoData *data);//ok
    bool modifyOneMusic(MusicInfoData *data);//ok
    bool deleteOneMusic(MusicInfoData *data);//no
    QList<MusicInfoData *> getAllFavourite();//ok
    bool isExist(MusicInfoData *data);//ok
    MusicInfoData * getOneMusic(const QString&titleQuery, const QString&albumQuery);
    QString getOneMusicPath(const QString& titleQuery, const QString& albumQuery);

signals:

public slots:

private:
    QSqlDatabase m_pDBMusic;


};

#endif // MUSICDBMANAGER_H
