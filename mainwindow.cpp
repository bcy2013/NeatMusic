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

#include<QUrl>
#include<QFileInfo>
#include<QDir>
#include<QStringList>
#include <QMediaPlayer>
#include<QVideoWidget>

#include <tag.h>
#include <fileref.h>
#include <tpropertymap.h>
#include <tag_c.h>
#include<iostream>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_MusicView(Q_NULLPTR),
    m_MusicInfoView(Q_NULLPTR)
{
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
    data->setItemIcon(QPixmap(":/Resources/btnSearch1.png"));
    data->setItemContent(QStringLiteral("朋友"));
    model->addItem(data);
    m_MusicView->setModel(model);
    m_MusicView->setCurrentIndex(model->index(1));
    //音乐列表
    m_MusicInfoView=static_cast<MusicInfoView *>(ui->listView_2);
    m_MusicInfoView->setItemDelegate(new MusicInfoListDelegrate(m_MusicInfoView));
    QList<MusicInfoData *> listMusic;
    MusicInfoData *data1;
    //读取配置文件里的音乐目录，并遍历所有文件，完成ｌｉｓｔＭｕｓｉｃ　ｍｏｄｅｌ的创建
    m_pSettings=new QSettings(QSettings::IniFormat, QSettings::UserScope,QStringLiteral("PersonProject"), QStringLiteral("NeatMusic"), this);
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
        string  path=str.replace("/","//").toStdString();
        data1=analyzeMusicInfo(path.data());
        listMusic.append(data1);
    };
    MusicListModel *musicListModel=new MusicListModel;
    musicListModel->addListMusicItem(listMusic);
    musicListModel->sort(0,Qt::AscendingOrder);
    m_MusicInfoView->setModel(musicListModel->mapToListViewDelegrate(musicListModel));
    QString strMusicCount=QString("%1%2").arg(QStringLiteral("歌曲")).arg(musicPathList.count());
    ui->tabWidget->setTabText(0,strMusicCount);
}

void MainWindow::setupSignalsSlots()
{
    connect(ui->toolButton_FileOpen,&QToolButton::clicked,this,&MainWindow::openMusicDirDlg);
    connect(ui->tBtn_PlayModel,&QToolButton::clicked,[this](){
        Toast *playModel=new Toast(this);
        playModel->resize(180,50);
        playModel->setPicture(false);
        playModel->setText(QStringLiteral("单曲循环"));
        playModel->pop();
        playModel->move(this->x()+(this->width()-playModel->width())/2,this->y()+(this->height()-playModel->height())/2);
    });
    connect(ui->tBtn_Love,&QToolButton::clicked,[this](){
        Toast *addLoveList=new Toast(this);
        addLoveList->resize(270,170);
        addLoveList->setPicture(true);
        addLoveList->setText(QStringLiteral("已添加到喜欢列表"));
        addLoveList->pop();
        addLoveList->move(this->x()+(this->width()-addLoveList->width())/2,this->y()+(this->height()-addLoveList->height())/2);
    });

    connect(ui->tBtn_PlayShow,&QToolButton::toggled,[this](){
        if(ui->tBtn_PlayShow->isChecked()==true)
           {
            m_pPlayMusicShow=new PlayMusicShow(this);
            //m_pPlayMusicShow->resize(this->geometry().width(),this->geometry().height()-60);
            // m_pPlayMusicShow->setFixedSize(0,0);
            m_pPlayMusicShow->toMax();
        }
        if(ui->tBtn_PlayShow->isChecked()==false)
        {
            m_pPlayMusicShow->toMin();
        }
    });
}

void MainWindow::setupMainWindow()
{

    ui->tBtn_PlayShow->setCheckable(true);
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

QString MainWindow::getTags(const char* path)
{
            QString string;

            TagLib::FileRef f(path);
            if(!f.isNull()){
                TagLib::Tag *tag = f.tag();
                QString title = QString::fromStdWString(tag->title().toWString());
                string = "标题: "+ title;
            }


            //TagLib::String album_string = file.tag()->album();
           // QString title = QString::fromStdWString(title_string.toWString());
            //QString  album = QString::fromStdWString(album_string.toCString());
            //QString artist = QString::fromStdWString(file.tag()->artist().toCString());
            //QString comment = QString::fromStdWString(file.tag()->comment().toCString());
            //QString genre = QString::fromStdWString(file.tag()->artist().toCString());
            //QString year = QString::number(file.tag()->year());
            //+"\n唱片集:"+album + " \n艺术家:" + artist+"\n注释:"+comment+ " \n流派: " + genre+"\n年份: "+year;
           // QString lengths = QString::number(file.audioProperties()->bitrate());
           // string.append("\n比特率: ");
           // string.append(lengths);
           // string.append("kbps");
            return string;
}
#include <stdio.h>
QString MainWindow::getTags_2(const char *path)
{
    QString str;
    int seconds;
    int minutes;
    TagLib_File *file;
    TagLib_Tag *tag;
    const TagLib_AudioProperties *properties;
    file = taglib_file_new(path);
    if(file == NULL)
      qDebug()<<"Error!!";
    tag = taglib_file_tag(file);
    properties = taglib_file_audioproperties(file);
    if(tag != NULL) {
      //printf("-- TAG --\n");
      //printf("title   - \"%s\"\n", taglib_tag_title(tag));
      string str3=taglib_tag_title(tag);
      str=QString::fromStdString(str3);
//      printf("title   - \"%s\"\n", taglib_tag_title(tag));
//      printf("artist  - \"%s\"\n", taglib_tag_artist(tag));
//      printf("album   - \"%s\"\n", taglib_tag_album(tag));
//      printf("year    - \"%i\"\n", taglib_tag_year(tag));
//      printf("comment - \"%s\"\n", taglib_tag_comment(tag));
//      printf("track   - \"%i\"\n", taglib_tag_track(tag));
//      printf("genre   - \"%s\"\n", taglib_tag_genre(tag));
    }

//    if(properties != NULL) {
//      seconds = taglib_audioproperties_length(properties) % 60;
//      minutes = (taglib_audioproperties_length(properties) - seconds) / 60;

//      printf("-- AUDIO --\n");
//      printf("bitrate     - %i\n", taglib_audioproperties_bitrate(properties));
//      printf("sample rate - %i\n", taglib_audioproperties_samplerate(properties));
//      printf("channels    - %i\n", taglib_audioproperties_channels(properties));
//      printf("length      - %i:%02i\n", minutes, seconds);
//    }

    taglib_tag_free_strings();
    taglib_file_free(file);
    return str;
}

MusicInfoData *MainWindow::analyzeMusicInfo(const char *path)
{
    MusicInfoData *data=new MusicInfoData();
    TagLib_File *file;
    TagLib_Tag *tag;
    const TagLib_AudioProperties *properties;

    file=taglib_file_new(path);
    tag=taglib_file_tag(file);
    properties = taglib_file_audioproperties(file);
    //infomation
    if(file!=NULL&&tag!=NULL){
        string temp=taglib_tag_title(tag);
        data->setTitle(QString::fromStdString(temp));//title
        temp=taglib_tag_artist(tag);
        data->setArtistName(QString::fromStdString(temp));//artist
        temp=taglib_tag_album(tag);
        data->setAlbumName(QString::fromStdString(temp));//album
    }
    if(file!=NULL&&properties!=NULL){
        int second=taglib_audioproperties_length(properties)%60;
        int minute=(taglib_audioproperties_length(properties)-second)/60;
        data->setDuration(QTime(0,minute,second));//duration
    }
    string pathTemp=path;
    QFileInfo dir(QString::fromStdString(pathTemp));
    if(dir.exists()){
        qint64 size=dir.size();
        data->setMusicSize(size/1024.0/1024.0);
    }

    taglib_tag_free_strings();
    taglib_file_free(file);
    return data;
}

void MainWindow::openMusicDirDlg()
{
    MusicDirDlg * p_MusicDirDlr=new MusicDirDlg(this);
    p_MusicDirDlr->resize(360,360);
    p_MusicDirDlr->setModal(true);
    p_MusicDirDlr->show();
}

