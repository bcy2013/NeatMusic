#include "mainwindow.h"
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
#include"playlistdelegrate.h"


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


const qint64 BufferDurationUs       = 10 * 1000000;
const int    NotifyIntervalMs       = 100;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_MusicView(Q_NULLPTR),
    m_MusicInfoView(Q_NULLPTR),
    m_dPlayListPlayMode(3),
    m_bIsAddLikeList(false),
    m_audioOutputDevice(QAudioDeviceInfo::defaultOutputDevice()),
    audioOutput(0),
    m_spectrumBufferLength(0),
    m_spectrumAnalyser(),
    m_dontShowUpdateWindow(false),
    m_bIsAutoStart(false)
{

    initDatabase();
    restoreStateSettings();
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
    data->setItemContent(QStringLiteral("音乐可视化"));
    model->addItem(data);
    data=new ListViewItemDate();
    data->setItemIcon(QPixmap(":/Resources/btnmusic2.png"));
    data->setItemContent(QStringLiteral("发现音乐"));
    model->addItem(data);
    data=new ListViewItemDate();
    data->setItemIcon(QPixmap(":/Resources/btnlist.png"));
    data->setItemContent(QStringLiteral("播放列表"));
    model->addItem(data);
    data=new ListViewItemDate();
    data->setItemIcon(QPixmap(":/Resources/btnradio2.png"));
    data->setItemContent(QStringLiteral("我喜欢的音乐"));
    model->addItem(data);
    m_MusicView->setModel(model);
    m_MusicView->setCurrentIndex(model->index(1));
    ui->topRightStackedWidget->setCurrentIndex(1);
    connect(ui->tBtn_PlayList,&QToolButton::clicked,[=](){
        ui->topRightStackedWidget->setCurrentIndex(2);
        m_MusicView->setCurrentIndex(model->index(2));
    });
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
        if(!m_pDbMusicManager->isExist(data1))
            QtConcurrent::run(m_pDbMusicManager,&MusicDbManager::addOneMusic,data1);
        listMusic.append(data1);
    };
    MusicListModel *musicListModel=new MusicListModel;
    musicListModel->addListMusicItem(listMusic);
    musicListModel->sort(0,Qt::AscendingOrder);
    m_MusicInfoView->setModel(musicListModel->mapToListViewDelegrate(musicListModel));

   // m_playlistView->setModel(musicListModel);
   // m_playlistView->setItemDelegate(new MusicViewDelegrate(m_playlistView));

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
   connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::ffmpeg_play);
//   connect(audioOutput,QOverload<QAudio::State>::of(&QAudioOutput::stateChanged),[=](QAudio::State state){
//           if(state==QAudio::IdleState)
//           {
//               //audioOutput->stop();
//               qDebug()<<"No Data!!";
//               ui->pushButton->setEnabled(true);
//           }
//   });

    connect(m_playlistView, &QAbstractItemView::activated, this, &MainWindow::jump);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::setAudioSuspend);
    connect(ui->toolButton_update,&QToolButton::clicked,[this]{
       m_pUpdateWindow.checkForUpdate(true);
    });

    connect(&m_pUpdateWindow,&updatewindow::dontShowUpdateWindowChanged,[=](bool state){
            m_dontShowUpdateWindow=state;
            m_pSettings->setValue("dontShowUpdateWindow", m_dontShowUpdateWindow);
    });

}

void MainWindow::setupMainWindow()

{

    checkForUpdateAuto();
    m_autoStart.setAutostart(m_bIsAutoStart);
    ui->tBtn_PlayShow->setCheckable(true);
    ui->tBtn_PlayList->setEnabled(ui->topRightStackedWidget->currentIndex()==2?false:true);
    musicPlayer=new QMediaPlayer(this);

    musicPlayer->setAudioRole(QAudio::MusicRole);
    m_pMediaPlayList=new QMediaPlaylist;

    m_pMediaPlayList->setPlaybackMode(QMediaPlaylist::PlaybackMode(m_dPlayListPlayMode));
    musicPlayer->setPlaylist(m_pMediaPlayList);
    m_pPlayListModel=new PlaylistModel(this);
    m_pPlayListModel->setPlaylist(m_pMediaPlayList);

    m_playlistView =static_cast<QListView *>(ui->listView_3);
    m_playlistView->setModel(m_pPlayListModel);
    m_playlistView->setItemDelegate(new PlayListDelegrate(m_playlistView));
    m_playlistView->setCurrentIndex(m_pPlayListModel->index(m_pMediaPlayList->currentIndex(),0));



    qRegisterMetaType<FrequencySpectrum>("FrequencySpectrum");
    qRegisterMetaType<WindowFunction>("WindowFunction");
    connect(&m_spectrumAnalyser, QOverload<const FrequencySpectrum&>::of(&SpectrumAnalyser::spectrumChanged),
            this, QOverload<const FrequencySpectrum&>::of(&MainWindow::spectrumChanged));
    m_spectrumAnalyser.setWindowFunction(WindowFunction::HannWindow);
    fmt.setSampleRate(44100);
    fmt.setSampleSize(16);
    fmt.setChannelCount(2);
    fmt.setCodec("audio/pcm");
    fmt.setByteOrder(QAudioFormat::LittleEndian);
    fmt.setSampleType(QAudioFormat::SignedInt);
    audioOutput = new QAudioOutput(m_audioOutputDevice, fmt, this);
    audioOutput->setNotifyInterval(NotifyIntervalMs);
    m_spectrumBufferLength = SpectrumLengthSamples *
                            (fmt.sampleSize() / 8) * fmt.channelCount();
    m_bufferLength = audioLength(fmt, BufferDurationUs);
    byteBuf.resize(m_bufferLength);
    byteBuf.fill(0);
    emit bufferLengthChanged(bufferLength());
    emit bufferChanged(0,0,byteBuf);
    m_pSpectrumWidget=static_cast<SpectrumWidget *>(ui->page_3);
    connect(this, QOverload<qint64, qint64, const FrequencySpectrum&>::of(&MainWindow::spectrumChanged),
            m_pSpectrumWidget, QOverload<qint64, qint64, const FrequencySpectrum&>::of(&SpectrumWidget::spectrumChanged));
}
static bool isPlaylist(const QUrl &url) // Check for ".m3u" playlists.
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}
#include<QListView>
void MainWindow::initMusicPlayControl()
{
    //init playList
    QList<MusicInfoData *> allMusicList=m_pDbMusicManager->getAllFavourite();
   // QList<QString> list=musicPathList.toList();
    for(int i=0;i<allMusicList.count();i++)
    {
        QUrl url=QUrl(allMusicList.at(i)->path());
        if(isPlaylist(url))
            m_pMediaPlayList->load(url);
        else{
            m_pMediaPlayList->addMedia(url);
        }
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
        QString str=allMusicList.at(m_pMediaPlayList->currentIndex())->path();
        string  path=str.toStdString();
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
        MusicInfoData *data=allMusicList.at(m_pMediaPlayList->currentIndex());

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
        int id=m_pDbMusicManager->getOneMusicID(currentTitle,currentAlbum);
        m_pMediaPlayList->setCurrentIndex(id-1);
        musicPlayer->play();
        ui->tBtn_Plsy->setIcon(QIcon(":/Resources/ooopic_1501575085.png"));
    });
    connect(m_pMediaPlayList,QOverload<int>::of(&QMediaPlaylist::currentIndexChanged),[=](int currentIndex){
       m_playlistView->setCurrentIndex(m_pPlayListModel->index(currentIndex,0));
       if(byteBuf.length()!=0)
           byteBuf.clear();
       ffmpeg_Stop();
       m_bufferLength=0;
    });
}

void MainWindow::initDatabase()
{
     m_pSettings=new QSettings(QSettings::IniFormat, QSettings::UserScope,QStringLiteral("PersonProject"), QStringLiteral("NeatMusic"), this);
     m_pSettings->setFallbacksEnabled(false);
     initSettingsDataBase();
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

void MainWindow::setWindowToFront(bool toFront)
{
    if(toFront){
        show();
        qApp->processEvents();
        qApp->setActiveWindow(this);
        qApp->processEvents();
    }else{
        hide();
    }
}

void MainWindow::setAudioSuspend()
{

        if (QAudio::ActiveState == audioOutput->state() ||
            QAudio::IdleState == audioOutput->state()) {
            switch (QAudio::AudioOutput) {
            case QAudio::AudioInput:
                audioOutput->suspend();
                break;
            case QAudio::AudioOutput:
                audioOutput->suspend();
                break;
            }
        }
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(false);
}

qint64 MainWindow::bufferLength()
{
    return m_bufferLength;
}

void MainWindow::setPlayPosition(qint64 position, bool forceEmit)
{
    const bool changed = (m_playPosition != position);
    m_playPosition = position;
    if (changed || forceEmit)
        emit playPositionChanged(m_playPosition);
}

void MainWindow::calculateSpectrum(qint64 position)
{
    //Q_ASSERT(position + m_spectrumBufferLength <= m_bufferPosition + m_dataLength);
    Q_ASSERT(0 == m_spectrumBufferLength % 2); // constraint of FFT algorithm

    // QThread::currentThread is marked 'for internal use only', but
    // we're only using it for debug output here, so it's probably OK :)

    if (m_spectrumAnalyser.isReady()) {
        m_spectrumBuffer = QByteArray::fromRawData(byteBuf.constData() + position,
                                                   m_spectrumBufferLength);
        m_spectrumPosition = position;
        m_spectrumAnalyser.calculate(m_spectrumBuffer, fmt);
    }
}

void MainWindow::initSettingsDataBase()
{
    if(m_pSettings->value("version", "NULL") == "NULL")
        m_pSettings->setValue("version", qApp->applicationVersion());
    if(m_pSettings->value("dontShowUpdateWindow", "NULL") == "NULL")
        m_pSettings->setValue("dontShowUpdateWindow", m_dontShowUpdateWindow);
    if(m_pSettings->value("autostart", "NULL") == "NULL")
        m_pSettings->setValue("autostart", m_bIsAutoStart);
}

void MainWindow::restoreStateSettings()
{
    if(m_pSettings->value("dontShowUpdateWindow", "NULL") != "NULL")
        m_dontShowUpdateWindow = m_pSettings->value("dontShowUpdateWindow").toBool();
    if(m_pSettings->value("autostart", "NULL") != "NULL")
        m_bIsAutoStart = m_pSettings->value("autostart").toBool();
}

void MainWindow::checkForUpdateAuto()
{
    m_pUpdateWindow.installEventFilter(this);
    m_pUpdateWindow.setShowWindowDisable(m_dontShowUpdateWindow);
    m_pUpdateWindow.checkForUpdate(false);
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

void MainWindow::getOneFram_FromThread(QByteArray ba)
{
    byteBuf.append(ba);
}

void MainWindow::ffmpeg_play()

{
     ui->pushButton->setEnabled(false);
     ui->pushButton_2->setEnabled(true);
     MusicPlayerDecoderThread* m_pPlayerDecoderThread = new MusicPlayerDecoderThread;
     connect(m_pPlayerDecoderThread, SIGNAL(get_One_Frame(QByteArray)), this, SLOT(getOneFram_FromThread(QByteArray)));
     QUrl path = m_pMediaPlayList->media(m_pMediaPlayList->currentIndex()).canonicalUrl();
     m_pPlayerDecoderThread->setAudioPath(path.toString());
     if(!m_pPlayerDecoderThread->isRunning()&&byteBuf.length()==0)
         m_pPlayerDecoderThread->start();
     connect(m_pPlayerDecoderThread, &MusicPlayerDecoderThread::finished, m_pPlayerDecoderThread, &QObject::deleteLater);
         if(audioOutput){
             if( QAudio::SuspendedState ==audioOutput->state())
             {
                  audioOutput->resume();
             }
             else{
                 m_spectrumAnalyser.cancelCalculation();
                 spectrumChanged(0, 0, FrequencySpectrum());
                 setPlayPosition(0, true);
                 connect(audioOutput,&QAudioOutput::stateChanged,this,&MainWindow::handleAudioOutputState);
                 connect(audioOutput,&QAudioOutput::notify,this,&MainWindow::handleNotify);
                 m_audioOutputIODevice.close();
                 m_audioOutputIODevice.setBuffer(&byteBuf);
                 qDebug()<<m_audioOutputIODevice.bytesAvailable();
                 m_audioOutputIODevice.open(QIODevice::ReadOnly);
                 audioOutput->start(&m_audioOutputIODevice);
             }
         }
}

void MainWindow::ffmpeg_Stop()
{
    if (audioOutput) {
        audioOutput->stop();
        qApp->processEvents();
        audioOutput->disconnect();
    }
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
}

void MainWindow::handleNotify()
{
    const qint64 playPosition = audioLength(fmt, audioOutput->processedUSecs());
    setPlayPosition(qMin(bufferLength(), playPosition));
    const qint64 spectrumPosition = playPosition - m_spectrumBufferLength;
   // if (spectrumPosition >= 0 && spectrumPosition + m_spectrumBufferLength < m_bufferPosition + m_dataLength)
        calculateSpectrum(spectrumPosition);
}

void MainWindow::handleAudioOutputState(QAudio::State)
{
    if (QAudio::IdleState == audioOutput->state()) {
        ffmpeg_Stop();
    } else {
        if (QAudio::StoppedState == audioOutput->state()) {
            // Check error
            QAudio::Error error = QAudio::NoError;
            switch (QAudio::AudioOutput) {
            case QAudio::AudioInput:
                error = audioOutput->error();
                break;
            case QAudio::AudioOutput:
                error = audioOutput->error();
                break;
            }
            if (QAudio::NoError != error) {
                //reset();
                return;
            }
        }
    }
}

void MainWindow::jump(const QModelIndex &index)
{
    m_pMediaPlayList->setCurrentIndex(index.row());
}

void MainWindow::spectrumChanged(const FrequencySpectrum &spectrum)
{
    emit spectrumChanged(m_spectrumPosition, m_spectrumBufferLength, spectrum);
}


bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    switch(event->type()){
    case QEvent::Show:
        if(watched == &m_pUpdateWindow){
            QRect rect = m_pUpdateWindow.geometry();
            QRect appRect = geometry();
            int titleBarHeight = 28 ;
            int x = appRect.x() + (appRect.width() - rect.width())/2.0;
            int y = appRect.y() + titleBarHeight  + (appRect.height() - rect.height())/2.0;
            m_pUpdateWindow.setGeometry(QRect(x, y, rect.width(), rect.height()));
        }
        break;
    default:
        break;
    }
    return QObject::eventFilter(watched, event);
}
