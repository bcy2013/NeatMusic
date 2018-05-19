#ifndef MUSICPLAYERDECODERTHREAD_H
#define MUSICPLAYERDECODERTHREAD_H

#include <QObject>
#include<QThread>
class MusicPlayerDecoderThread : public QThread
{
    Q_OBJECT
public:
    explicit MusicPlayerDecoderThread(QObject *parent = nullptr);
    ~MusicPlayerDecoderThread();
    void setAudioPath(const QString& path);

signals:
    void get_One_Frame(QByteArray);
public slots:

    // QThread interface
protected:
    void run() Q_DECL_OVERRIDE;
private:
    QString m_strAudioPath;
};

#endif // MUSICPLAYERDECODERTHREAD_H
