﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QSettings>
#include"musicview.h"
#include"musicinfoview.h"

#include"musicdirdlg.h"
#include"playmusicshow.h"
#include"musicdbmanager.h"


#include<QStringList>
#include<QMediaPlaylist>
#include<QMediaPlayer>
#include<QSet>
#include<QtConcurrent>
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
signals:
    void dataBaseChanged();
private:
    Ui::MainWindow *ui;
    MusicView *m_MusicView;
    MusicInfoView *m_MusicInfoView;
    PlayMusicShow *m_pPlayMusicShow;
    QSettings *m_pSettings;

    MusicDbManager *m_pDbMusicManager;
    int m_intCount;

    QSet<QString> musicPathList;
    QMediaPlaylist *m_pMediaPlayList;
    QMediaPlayer *musicPlayer=NULL;
    qint64 m_Duration;
    int m_dPlayListPlayMode;
    bool m_bIsAddLikeList;
    QImage albumImg;
};

#endif // MAINWINDOW_H
