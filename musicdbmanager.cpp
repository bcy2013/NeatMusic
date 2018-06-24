#include "musicdbmanager.h"
#include<QtSql/QSqlQuery>
#include<QDebug>
MusicDbManager::MusicDbManager(const QString &path, const bool &doWhat, QObject *parent)
    :QObject(parent)
{
    m_pDBMusic=QSqlDatabase::addDatabase("QSQLITE");
    m_pDBMusic.setDatabaseName(path);
    if(!m_pDBMusic.open()){
        qDebug()<<"Error:Connection Failed!";
    }else{
        qDebug()<<"Info:Connetcion Succeeded!";
    }
    if(doWhat){
        QSqlQuery query(m_pDBMusic);
        QString createDBMusic=("CREATE TABLE all_music_info(id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "                              title VARCHAR,"
        "                              artist VARCHAR,"
        "                              album VARCHAR,"
        "                              duration TEXT,"
        "                              favourite INIEGER,"
        "                              size REAL,"
        "                              path TEXT);"
                               );
        query.exec(createDBMusic);
    }
}

bool MusicDbManager::addOneMusic(MusicInfoData *data)
{
    QSqlQuery query(m_pDBMusic);
    QString emptyStr;
    QString title=data->title()
            .replace("'","''")
            .replace(QChar('\x0'), emptyStr);
    QString artist=data->artistName()
            .replace("'","''")
            .replace(QChar('\x0'), emptyStr);
    QString album=data->albumName()
            .replace("'","''")
            .replace(QChar('\x0'), emptyStr);
    QString duration=data->duration().toString("mm:ss");
    qreal size=data->musicSize();
    bool favourite=data->isFavourite();
    QString path=data->path()
            .replace("'","''")
            .replace(QChar('\x0'), emptyStr);
    QString queryStr=QString("INSERT INTO all_music_info(title,artist,album,duration,favourite,size,path) VALUES('%1','%2','%3','%4',%5,%6,'%7');")
            .arg(title)
            .arg(artist)
            .arg(album)
            .arg(duration)
            .arg(favourite)
            .arg(size)
            .arg(path);
    query.exec(queryStr);
    if (this->parent() == Q_NULLPTR) {
        delete data;
    }
    return (query.numRowsAffected()==1);
}

bool MusicDbManager::modifyOneMusic(MusicInfoData *data)
{
    QSqlQuery query;
    bool modify_favourite=data->isFavourite();
    QString queryStr=QStringLiteral("UPDATE all_music_info SET favourite=%1 WHERE title='%2' AND album='%3';").arg(modify_favourite).arg(data->title()).arg(data->albumName());
    query.exec(queryStr);
    return (query.numRowsAffected()==1);
}

bool MusicDbManager::deleteOneMusic(MusicInfoData *data)
{
    return true;
}

QList<MusicInfoData *> MusicDbManager::getAllFavourite()
{   QList<MusicInfoData *> musciFavouriteList;
    QSqlQuery query;
    QString queryStr=QStringLiteral("SELECT * FROM all_music_info ;");
    query.prepare(queryStr);
    bool state=query.exec();
    if(state){
        while (query.next()) {
            MusicInfoData *data=new MusicInfoData(this);

            QString data_title=query.value(1).toString();
            QString data_artist=query.value(2).toString();
            QString data_album=query.value(3).toString();
            QTime data_duration=QTime::fromString(query.value(4).toString());
            bool data_favourite=query.value(5).toBool();
            qreal data_size=query.value(6).toDouble();
            QString data_path=query.value(7).toString();

            data->setTitle(data_title);
            data->setArtistName(data_artist);
            data->setAlbumName(data_album);
            data->setDuration(data_duration);
            data->setIsFavourite(data_favourite);
            data->setMusicSize(data_size);
            data->setPath(data_path);
            musciFavouriteList.push_back(data);
        }
//xin hao
    }
    return musciFavouriteList;

}

bool MusicDbManager::isExist(MusicInfoData *data)
{
    QSqlQuery query;
    QString title=data->title();
    QString album=data->albumName();
    QString queryStr=QStringLiteral("SELECT EXISTS(SELECT 1 FROM all_music_info WHERE title='%1' AND album='%2') LIMIT 1;").arg(title).arg(album);
    query.exec(queryStr);
    query.next();
    return query.value(0).toInt()==1;
}
int MusicDbManager::getLastRowID()
{
    QSqlQuery query;
    query.exec("SELECT seq from SQLITE_SEQUENCE WHERE name='all_music_info';");
    query.next();
    return query.value(0).toInt();
}

MusicInfoData *MusicDbManager::getOneMusic(const QString &titleQuery, const QString &albumQuery)
{
    QSqlQuery query;
    QString queryStr=QStringLiteral("SELECT * FROM all_music_info WHERE title='%1' AND album='%2' LIMIT 1;").arg(titleQuery).arg(albumQuery);
    query.exec(queryStr);
    if(query.first()){
        MusicInfoData *data=new MusicInfoData(this);
        QString data_title=query.value(1).toString();
        QString data_artist=query.value(2).toString();
        QString data_album=query.value(3).toString();
        QTime data_duration=QTime::fromString(query.value(4).toString());
        bool data_favourite=query.value(5).toBool();
        qreal data_size=query.value(6).toDouble();
        QString data_path=query.value(7).toString();
        data->setTitle(data_title);
        data->setArtistName(data_artist);
        data->setAlbumName(data_album);
        data->setDuration(data_duration);
        data->setIsFavourite(data_favourite);
        data->setMusicSize(data_size);
        data->setPath(data_path);
        return data;
    }
    return Q_NULLPTR;
}

int MusicDbManager::getOneMusicID(const QString &titleQuery, const QString &albumQuery)
{
    QSqlQuery query;
    QString queryStr=QStringLiteral("SELECT id FROM all_music_info WHERE title='%1' AND album='%2' LIMIT 1;").arg(titleQuery).arg(albumQuery);
    query.exec(queryStr);
    if(query.first()){
        int id=query.value(0).toInt();
        return id;
    }
    return -1;

}
