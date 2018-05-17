﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

#include"musicviewdelegrate.h"
#include"listviewitemmodel.h"
#include"listviewitemdate.h"

#include"musicinfolistdelegrate.h"
#include"musicinfodata.h"
#include"musiclistmodel.h"
#include"toast.h"
#include"playmusicshow.h"
#include"albumshow.h"


#include<QUrl>
#include<QFileInfo>
#include<QDir>
#include<QStringList>
#include<QMediaMetaData>
#include<QMediaPlayer>
#include<QMediaPlaylist>

#include<iostream>
using namespace std;

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
#include "libavutil/dict.h"
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_MusicView(Q_NULLPTR),
    m_MusicInfoView(Q_NULLPTR),
   m_dPlayListPlayMode(3),
   m_bIsAddLikeList(false)
{

    initDatabase();
    ui->setupUi(this);
    setupMainWindow();

    setModelView();
    setupSignalsSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}
#include<QDebug>
void MainWindow::setModelView()
{  //左侧列表项按钮
    m_MusicView=static_cast<MusicView *>(ui->listView);
    m_MusicView->setItemDelegate(new MusicViewDelegrate(m_MusicView));
    ListViewItemModel *model=new ListViewItemModel(this);
    ListViewItemDate *data=new ListViewItemDate();
    data->setItemIcon(QPixmap(":/Resources/btnSearch1.png"));
    data->setItemContent(QStringLiteral("搜索"));
    model->addItem(data);
    data=new ListViewItemDate();
    data->setItemIcon(QPixmap(":/Resources/btnmusic2.png"));
    data->setItemContent(QStringLiteral("发现音乐"));
    model->addItem(data);
    data=new ListViewItemDate();
    data->setItemIcon(QPixmap(":/Resources/btnmv.png"));
    data->setItemContent("MV");
    model->addItem(data);
    data=new ListViewItemDate();
    data->setItemIcon(QPixmap(":/Resources/btnmusic2.png"));
    data->setItemContent(QStringLiteral("我喜欢的音乐"));
    model->addItem(data);
    m_MusicView->setModel(model);
    m_MusicView->setCurrentIndex(model->index(1));
    ui->topRightStackedWidget->setCurrentIndex(1);
    //音乐列表
    m_MusicInfoView=static_cast<MusicInfoView *>(ui->listView_2);
    m_MusicInfoView->setItemDelegate(new MusicInfoListDelegrate(m_MusicInfoView));
    QList<MusicInfoData *> listMusic;
    MusicInfoData *data1;
    //读取配置文件里的音乐目录，并遍历所有文件，完成ｌｉｓｔＭｕｓｉｃ　ｍｏｄｅｌ的创建
    QString urls=m_pSettings->value("Urls").toString();
    QStringList strList=urls.split(';');
    int temp=strList.removeAll("");
    for(int i=0;i<strList.size();i++){
        getAllMusics(strList.at(i));
    }
    QList<QString> list=musicPathList.toList();
    for(int i=0;i<list.count();i++)
    {
        QString str=list.at(i);
        if(str.isEmpty())
            continue;
        std::string  path=str.replace("/","//").toStdString();
        data1=analyzeMusicInfo(path.c_str(),false);
        data1->setPath(str);
        //m_pDbMusicManager->addOneMusic(data1);
        if(!m_pDbMusicManager->isExist(data1))
            QtConcurrent::run(m_pDbMusicManager,&MusicDbManager::addOneMusic,data1);
        listMusic.append(data1);
    };
    MusicListModel *musicListModel=new MusicListModel;
    musicListModel->addListMusicItem(listMusic);
    musicListModel->sort(0,Qt::AscendingOrder);
    m_MusicInfoView->setModel(musicListModel->mapToListViewDelegrate(musicListModel));
    QString strMusicCount=QString("%1%2").arg(QStringLiteral("歌曲")).arg(musicPathList.count());
    ui->tabWidget->setTabText(0,strMusicCount);

    initMusicPlayControl();
}

void MainWindow::setupSignalsSlots()
{
    connect(ui->toolButton_FileOpen,&QToolButton::clicked,this,&MainWindow::openMusicDirDlg);
    connect(ui->tBtn_PlayModel,&QToolButton::clicked,[this](){
        Toast *playModel=new Toast(this);
        playModel->resize(180,50);
        playModel->setPicture(false);
        m_dPlayListPlayMode+=1;
        m_pMediaPlayList->setPlaybackMode(QMediaPlaylist::PlaybackMode(m_dPlayListPlayMode));
        switch(m_dPlayListPlayMode%5){
        case 0:
             playModel->setText(QStringLiteral("单曲播放"));
             ui->tBtn_PlayModel->setIcon(QIcon(":/Resources/player-icons_32px_1137006_easyicon.net.png"));
             break;
        case 1:
             playModel->setText(QStringLiteral("单曲循环"));
             ui->tBtn_PlayModel->setIcon(QIcon(":/Resources/listsingle.png"));
            break;
        case 2:
             playModel->setText(QStringLiteral("顺序播放"));
              ui->tBtn_PlayModel->setIcon(QIcon(":/Resources/listsequence.png"));
            break;
        case 3:
             playModel->setText(QStringLiteral("列表循环"));
             ui->tBtn_PlayModel->setIcon(QIcon(":/Resources/listcircle.png"));
            break;
        case 4:
             playModel->setText(QStringLiteral("随机播放"));
              ui->tBtn_PlayModel->setIcon(QIcon(":/Resources/listrandom.png"));
            break;
        }
        playModel->pop();
        playModel->move(this->x()+(this->width()-playModel->width())/2,this->y()+(this->height()-playModel->height())/2);
    });


    connect(ui->tBtn_PlayShow,&QToolButton::toggled,[this](){
        if(ui->tBtn_PlayShow->isChecked()==true)
           {
            m_pPlayMusicShow=new PlayMusicShow(this);
            //m_pPlayMusicShow->resize(this->geometry().width(),this->geometry().height()-60);
            m_pPlayMusicShow->toMax();
        }
        if(ui->tBtn_PlayShow->isChecked()==false)
        {
            m_pPlayMusicShow->toMin();
        }
    });

    connect(ui->listView,&MusicView::clicked,[this](){
        ui->topRightStackedWidget->setCurrentIndex(ui->listView->currentIndex().row());
    });

   connect(this,&MainWindow::dataBaseChanged,ui->page_2,&Favourite::updateView);
}

void MainWindow::setupMainWindow()
{

    ui->tBtn_PlayShow->setCheckable(true);
    musicPlayer=new QMediaPlayer(this);

    musicPlayer->setAudioRole(QAudio::MusicRole);
    m_pMediaPlayList=new QMediaPlaylist;

    m_pMediaPlayList->setPlaybackMode(QMediaPlaylist::PlaybackMode(m_dPlayListPlayMode));
    m_pMediaPlayList->setCurrentIndex(0);
    musicPlayer->setPlaylist(m_pMediaPlayList);
}
static bool isPlaylist(const QUrl &url) // Check for ".m3u" playlists.
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}
void MainWindow::initMusicPlayControl()
{
    //init playList
    QList<QString> list=musicPathList.toList();
    for(int i=0;i<list.count();i++)
    {
        QUrl url=QUrl(list.at(i));
        if(isPlaylist(url))
            m_pMediaPlayList->load(url);
        else
             m_pMediaPlayList->addMedia(url);
    };

    ui->tBtn_PlayList->setText(QString::number(m_pMediaPlayList->mediaCount()));
    connect(ui->tBtn_Plsy,&QToolButton::clicked,[this](){
        switch (musicPlayer->state()) {
        case QMediaPlayer::StoppedState:
        case QMediaPlayer::PausedState:
            musicPlayer->play();
            ui->tBtn_Plsy->setIcon(QIcon(":/Resources/ooopic_1501575085.png"));
            break;
        case QMediaPlayer::PlayingState:
            musicPlayer->pause();
            ui->tBtn_Plsy->setIcon(QIcon(":/Resources/ooopic_1501575097.png"));
            break;
        }
    }); 
    connect(ui->tBtn_Next,&QToolButton::clicked,m_pMediaPlayList,&QMediaPlaylist::next);
    connect(ui->tBtn_Pre,&QToolButton::clicked,m_pMediaPlayList,&QMediaPlaylist::previous);
    connect(musicPlayer, QOverload<>::of(&QMediaObject::metaDataChanged),[=](){
        QString str=list.at(m_pMediaPlayList->currentIndex());
        string  path=str.replace("/","//").toStdString();
        MusicInfoData *data=analyzeMusicInfo(path.c_str(),true);
        QString strMusicname=data->title();
        QString strMusicArtist=data->artistName();
        QString sttMusicAlbum=data->albumName();
        if(strMusicname.isEmpty())
            strMusicname=QStringLiteral("未知");
        if(strMusicArtist.isEmpty())
            strMusicArtist=QStringLiteral("未知");
        ui->label_musicName->setText(strMusicname+" - "+strMusicArtist);
        m_bIsAddLikeList=(m_pDbMusicManager->getOneMusic(strMusicname,sttMusicAlbum))->isFavourite();
        if((m_pDbMusicManager->getOneMusic(strMusicname,sttMusicAlbum))->isFavourite())
            ui->tBtn_Love->setIcon(QIcon(":/Resources/like_32px_1101682_easyicon.net.png"));
        else
            ui->tBtn_Love->setIcon(QIcon(":/Resources/like_outline_32px_1170275_easyicon.net.png"));
    });
//    connect(musicPlayer,QOverload<const bool&>::of(&QMediaObject::metaDataAvailableChanged),[=](const bool& availibal){
//        if(availibal)
//        {
//            qDebug()<<"Succeed!!"<<endl;
//            QString strMusicname=musicPlayer->metaData(QMediaMetaData::Title).toString();
//            ui->label_musicName->setText(strMusicname);
//        }
//    });
//    connect(musicPlayer,&QMediaObject::metaDataAvailableChanged,[this](){
//        if(musicPlayer->isMetaDataAvailable())
//        {
//            qDebug()<<"Succeed!!"<<endl;
//            QString strMusicname=musicPlayer->metaData(QMediaMetaData::Title).toString();
//            ui->label_musicName->setText(strMusicname);
//        }
//    });
    connect(musicPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),[this](){
        qDebug()<<musicPlayer->errorString();
    });
    connect(musicPlayer, &QMediaPlayer::durationChanged,[this](){
        m_Duration=musicPlayer->duration()/1000;
        ui->music_Slider->setRange(0,m_Duration);
    });
    connect(musicPlayer, &QMediaPlayer::positionChanged, [this](){
        qint64 progress=musicPlayer->position()/1000;
        if (!ui->music_Slider->isSliderDown())
            ui->music_Slider->setValue(progress);
        QString tStr;
        if (progress || m_Duration) {
            QTime currentTime((progress / 3600) % 60, (progress / 60) % 60,
                progress % 60, (progress * 1000) % 1000);
            QTime totalTime((m_Duration / 3600) % 60, (m_Duration / 60) % 60,
                m_Duration % 60, (m_Duration * 1000) % 1000);
            QString format = "mm:ss";
            if (m_Duration > 3600)
                format = "hh:mm:ss";
            tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
        }
        ui->label_Duration->setText(tStr);
    });
    connect(ui->music_Slider,QOverload<int>::of(&QSlider::sliderMoved), this, &MainWindow::seek);
    connect(ui->tBtn_Love,&QToolButton::clicked,[=](){
        Toast *addLoveList=new Toast(this);
        m_bIsAddLikeList=!m_bIsAddLikeList;
        QString str=list.at(m_pMediaPlayList->currentIndex());
        string  path=str.replace("/","//").toStdString();
        MusicInfoData *data=analyzeMusicInfo(path.c_str(),false);

        if(m_bIsAddLikeList){
            addLoveList->resize(270,170);
            addLoveList->setPicture(true);
            addLoveList->setText(QStringLiteral("已添加到喜欢列表"));
            ui->tBtn_Love->setIcon(QIcon(":/Resources/like_32px_1101682_easyicon.net.png"));

        }else{
            addLoveList->resize(180,50);
            addLoveList->setPicture(false);
            addLoveList->setText(QStringLiteral("以从喜欢列表中移除"));
             ui->tBtn_Love->setIcon(QIcon(":/Resources/like_outline_32px_1170275_easyicon.net.png"));
        }
        data->setIsFavourite(m_bIsAddLikeList);
        m_pDbMusicManager->modifyOneMusic(data);
        emit dataBaseChanged();
        addLoveList->pop();
        addLoveList->move(this->x()+(this->width()-addLoveList->width())/2,this->y()+(this->height()-addLoveList->height())/2);
    });
    connect(ui->listView_2,&MusicView::doubleClicked,[=](){
        QString currentTitle=ui->listView_2->currentIndex().data(MusicListModel::title).toString();
        QString currentAlbum=ui->listView_2->currentIndex().data(MusicListModel::album).toString();
        QString path=m_pDbMusicManager->getOneMusicPath(currentTitle,currentAlbum);
        int id=list.indexOf(path.replace("//","/"));
        m_pMediaPlayList->setCurrentIndex(id);
        musicPlayer->play();
        ui->tBtn_Plsy->setIcon(QIcon(":/Resources/ooopic_1501575085.png"));
    });
}

void MainWindow::initDatabase()
{
     m_pSettings=new QSettings(QSettings::IniFormat, QSettings::UserScope,QStringLiteral("PersonProject"), QStringLiteral("NeatMusic"), this);
     m_pSettings->setFallbacksEnabled(false);
     //initSettingsDataBase();
     bool doWhat=false;
     QFileInfo fi(m_pSettings->fileName());
     QDir dir(fi.absolutePath());
     bool foldCreated=dir.mkpath(QStringLiteral("."));
     if(!foldCreated)
         qDebug()<<"FoldCreate Failed!!";
     QString dbPath(dir.path()+QStringLiteral("/music.db"));
     if(!QFile::exists(dbPath)){
         QFile dbFile(dbPath);
         if(!dbFile.open(QIODevice::WriteOnly)){
             qFatal("ERROR : Can't create database file");
         }
         dbFile.close();
         doWhat=true;
     }
    m_pDbMusicManager=new MusicDbManager(dbPath,doWhat,this);
    //m_intCount=m_pDbMusicManager->getLastRowID();
}

void MainWindow::getAllMusics(const QString &path)
{
        QDir dir(path);
        if(dir.exists()){
            QStringList filters;
            filters << QString("*.mp3")<<QString("*.wav")<<QString("*.flac");
            dir.setFilter(QDir::Files| QDir::NoSymLinks|QDir::AllDirs);
            dir.setNameFilters(filters);
            if(dir.count()>0){
                    //获取分隔符
                    //QChar separator = QDir::separator();
                    QChar separator = QChar('/');
                    if(!path.contains(separator))
                    {
                        separator = QChar('\\');
                    }
                    QChar last_char = path.at(path.length()-1);
                    if(last_char == separator)
                    {
                        separator = QChar();
                    }
                    foreach(QFileInfo mfi ,dir.entryInfoList())
                    {
                        if(mfi.isFile())
                        {
                            QString file_path = path + separator +  mfi.fileName();
                            musicPathList.insert(file_path);
                        }else {
                           if(mfi.fileName()=="." || mfi.fileName() == "..")continue;
                            getAllMusics(mfi.absoluteFilePath());
                        }
                    }
            }
        }
}
#include"pinyin/pinyinhelper.h"
MusicInfoData *MainWindow::analyzeMusicInfo(const char *path,bool isAnalyzePicture)
{
    MusicInfoData *data=new MusicInfoData();
    av_register_all();
    AVFormatContext *fmt_ctx = NULL;
    AVDictionaryEntry *tag = NULL;
    int ret = avformat_open_input(&fmt_ctx, path, NULL, NULL);
    while (!ret&&(tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))){
       std::string temp_key=tag->key;
       std::string temp_value=tag->value;
       QString key=QString::fromStdString(temp_key);
       QString value=QString::fromStdString(temp_value);
        if(key=="title")
        {
            data->setTitle(value);
            continue;
        }
        if(key=="artist"){
            data->setArtistName(value);
            continue;
        }

        if(key=="album"){
            data->setAlbumName(value);
            continue;
        }

    }
        avformat_find_stream_info(fmt_ctx, NULL);
        qint64 duration=fmt_ctx->duration/1000/1000;//ms->s
        int second=duration%60;
        int minute=(duration-second)/60;
        data->setDuration(QTime(0,minute,second));//duration

    string pathTemp=path;
    QFileInfo dir(QString::fromStdString(pathTemp));
    if(dir.exists()){
        qint64 size=dir.size();
        data->setMusicSize(size/1024.0/1024.0);
    }
      if(isAnalyzePicture){
          if (fmt_ctx->iformat->read_header(fmt_ctx) < 0) {
                      printf("No header format");
                  }
           for (int i = 0; i < fmt_ctx->nb_streams; i++){
                          if (fmt_ctx->streams[i]->disposition & AV_DISPOSITION_ATTACHED_PIC) {
                              AVPacket pkt = fmt_ctx->streams[i]->attached_pic;
                              albumImg = QImage::fromData((uchar*)pkt.data, pkt.size);
                              QString path= QDir::currentPath()+"//cover.jpg";
                              albumImg.save(path);
                              ui->tBtn_PlayShow->setIcon(QIcon(QPixmap::fromImage(albumImg)));
                              break;
           }
      }
  }
    avformat_close_input(&fmt_ctx);
    avformat_free_context(fmt_ctx);
    return data;
}

void MainWindow::openMusicDirDlg()
{
    MusicDirDlg * p_MusicDirDlr=new MusicDirDlg(this);
    p_MusicDirDlr->resize(360,360);
    p_MusicDirDlr->setModal(true);
    p_MusicDirDlr->show();
}

void MainWindow::seek(int second)
{
    musicPlayer->setPosition(second*1000);
}

