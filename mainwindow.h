#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QSettings>
#include"musicview.h"
#include"musicinfoview.h"

#include"musicdirdlg.h"
#include"playmusicshow.h"
#include<QStringList>
#include<QMediaPlayer>
#include<QSet>
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

    void getAllMusics(const QString& path);
    QString getTags(const char *path);
    QString getTags_2(const char *path);
    MusicInfoData * analyzeMusicInfo(const char *path);
private slots:
    void openMusicDirDlg();
signals:

private:
    Ui::MainWindow *ui;
    MusicView *m_MusicView;
    MusicInfoView *m_MusicInfoView;
    PlayMusicShow *m_pPlayMusicShow;
    QSettings *m_pSettings;
    QSet<QString> musicPathList;
    QMediaPlayer *musicPlayer;
};

#endif // MAINWINDOW_H
