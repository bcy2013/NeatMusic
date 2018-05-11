#ifndef MUSICINFODATA_H
#define MUSICINFODATA_H

#include <QObject>
#include<QTime>
#include<QDataStream>
class MusicInfoData : public QObject
{
    Q_OBJECT
public:
    explicit MusicInfoData(QObject *parent = nullptr);
    explicit MusicInfoData(QString title,QString artist,QTime duration,QString album,qreal musicSize);
    ~MusicInfoData();

    QString title()const;
    QString artistName()const;
    QTime duration()const;
    QString albumName()const;
    qreal musicSize()const;
     int parentRow()const;
    void setTitle(const QString& title);
    void setArtistName(const QString& artistName);
    void setDuration(const QTime& duration);
    void setAlbumName(const QString& albumName);
    void setMusicSize(const qreal& musicSize);
    void setParentRow(const int& parentRow);


signals:

public slots:
private:
  QString m_strTitle;
  QString m_strArtistName;
  QTime m_tDuration;
  QString m_strAlbumName;
  qreal m_fMusicSize;
  int m_iParentRow;
};
QDataStream &operator<<(QDataStream &stream, const MusicInfoData* listData);
QDataStream &operator>>(QDataStream &stream, MusicInfoData *&listData);
#endif // MUSICINFODATA_H
