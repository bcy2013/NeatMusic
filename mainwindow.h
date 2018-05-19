#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QSettings>
#include"musicview.h"
#include"musicinfoview.h"

#include"musicdirdlg.h"
#include"playmusicshow.h"
#include"musicdbmanager.h"
#include"playlistmodel.h"
#include"musicplayerdecoderthread.h"
#include<QStringList>
#include<QMediaPlaylist>
#include<QMediaPlayer>
#include<QSet>
#include<QtConcurrent>
#include<QAudioOutput>
QT_BEGIN_NAMESPACE
class QAbstractItemView;
QT_END_NAMESPACE
class MusicInfoData;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setModelView();
    void setupSignalsSlots();
    void setupMainWindow();
    void initMusicPlayControl();
    void initDatabase();
    void getAllMusics(const QString& path);
    MusicInfoData * analyzeMusicInfo(const char *path,bool isAnalyzePicture);
    //MusicInfoData * analyzeMusicInfo_ffmpeg(const char *path);

private slots:
    void openMusicDirDlg();
    void seek(int second);
    void getOneFram_FromThread(QByteArray ba);
    void ffmpeg_play();
    void handleAudioOutputState(QAudio::State);
    void jump(const QModelIndex& index);
signals:
    void dataBaseChanged();
private:
    Ui::MainWindow *ui;
    MusicView *m_MusicView;
    MusicInfoView *m_MusicInfoView;
    PlayMusicShow *m_pPlayMusicShow;
    QSettings *m_pSettings;
    PlaylistModel *m_pPlayListModel;
    QAbstractItemView *m_playlistView;
    MusicDbManager *m_pDbMusicManager;
    int m_intCount;

    QSet<QString> musicPathList;
    QMediaPlaylist *m_pMediaPlayList;
    QMediaPlayer *musicPlayer=NULL;
    qint64 m_Duration;
    int m_dPlayListPlayMode;
    bool m_bIsAddLikeList;
    QImage albumImg;

    //解码部分
    QByteArray byteBuf;//音频缓冲
    QIODevice *streamOut;
    QAudioOutput *audioOutput;

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
};

#endif // MAINWINDOW_H
