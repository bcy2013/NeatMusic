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
#include"spectrumwidget.h"
#include"spectrumanalyser.h"
#include<QAutostart>
#include<QStringList>
#include<QMediaPlaylist>
#include<QMediaPlayer>
#include<QSet>
#include<QtConcurrent>
#include<QAudioOutput>
#include<QBuffer>
#include<QAudioDeviceInfo>
#include<QAudioFormat>
#include"updatewindow.h"
class QAbstractItemView;
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
    void setWindowToFront(bool toFront);
    void setAudioSuspend();
    qint64 bufferLength();
    void setPlayPosition(qint64 position, bool forceEmit = false);
    void calculateSpectrum(qint64 position);
    void initSettingsDataBase();
    void restoreStateSettings();
    //MusicInfoData * analyzeMusicInfo_ffmpeg(const char *path);

public slots:
    void checkForUpdateAuto();
    void openMusicDirDlg();
    void seek(int second);
    void getOneFram_FromThread(QByteArray ba);
    void ffmpeg_play();
    void ffmpeg_Stop();
    void handleNotify();
    void handleAudioOutputState(QAudio::State);
    void jump(const QModelIndex& index);
    void spectrumChanged(const FrequencySpectrum &spectrum);
signals:
    void dataBaseChanged();
    void bufferLengthChanged(qint64 duration);
    void bufferChanged(qint64 position, qint64 length, const QByteArray &buffer);
    void playPositionChanged(qint64 position);
    void spectrumChanged(qint64 position, qint64 length, const FrequencySpectrum &spectrum);
private:
    Ui::MainWindow *ui;
    MusicView *m_MusicView;
    MusicInfoView *m_MusicInfoView;
    PlayMusicShow *m_pPlayMusicShow;
    QSettings *m_pSettings;
    PlaylistModel *m_pPlayListModel;
    QAbstractItemView *m_playlistView;
    MusicDbManager *m_pDbMusicManager;
    updatewindow m_pUpdateWindow;
    int m_intCount;
    bool m_dontShowUpdateWindow;

    QSet<QString> musicPathList;
    QMediaPlaylist *m_pMediaPlayList;
    QMediaPlayer *musicPlayer=NULL;
    qint64 m_Duration;
    int m_dPlayListPlayMode;
    bool m_bIsAddLikeList;
    QImage albumImg;

    //解码部分
    QByteArray byteBuf;//音频缓冲
    //qint64              m_bufferPosition;
    qint64              m_bufferLength;
    //qint64              m_dataLength;
    qint64              m_playPosition;

   //输出部分
    QAudioDeviceInfo m_audioOutputDevice;
    QAudioOutput *audioOutput;
    QAudioFormat fmt;

    //频谱部分
    int                 m_spectrumBufferLength;
    QByteArray          m_spectrumBuffer;
    SpectrumAnalyser    m_spectrumAnalyser;
    qint64              m_spectrumPosition;




    SpectrumWidget *m_pSpectrumWidget;
    QBuffer        m_audioOutputIODevice;

    Autostart m_autoStart;
    bool m_bIsAutoStart;
    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
};

#endif // MAINWINDOW_H
