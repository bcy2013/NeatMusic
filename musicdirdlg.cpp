#include "musicdirdlg.h"
#include "ui_musicdirdlg.h"
#include<QFile>
#include<QGraphicsDropShadowEffect>
#include<QPainter>
#include<QFileDialog>
#include<QStandardPaths>
#include<QListWidgetItem>
#include<QCursor>
MusicDirDlg::MusicDirDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MusicDirDlg)
{
    ui->setupUi(this);
    initDialog();
    initConnections();
    loadStyleSheet(":/qss/musicDirDlg.css");
}

MusicDirDlg::~MusicDirDlg()
{
    delete ui;
}

void MusicDirDlg::initDialog()
{
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog
                         );
    this->setAttribute(Qt::WA_TranslucentBackground);
    ui->listWidget->setCurrentRow(0);
    loadStoredDir();


//    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
//    effect->setBlurRadius(20);
//    effect->setColor(QColor(63, 63, 63, 180));
//    effect->setOffset(8);
//    this->setGraphicsEffect(effect);
}
#include<QSettings>
#include<QMessageBox>
#include<QDebug>
void MusicDirDlg::initConnections()
{
    connect(ui->closeBtn,&QPushButton::clicked,[this](){
        this->close();
    });

    connect(ui->btn_ADD,&QPushButton::clicked,[this](){
        QString dir=QFileDialog::getExistingDirectory(this,tr("Open Directory","/home",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks));
        if(!m_pMusicUrls.contains(dir)){
            m_pMusicUrls.insert(dir);
            updateListView(m_pMusicUrls);
        }else{
            QMessageBox::information(this, "Warming", "Exits!!");
        }
    });
    connect(ui->btn_OK,&QPushButton::clicked,this,&MusicDirDlg::saveAllExitDir);
}

void MusicDirDlg::loadStyleSheet(const QString &qssDir)
{
    QFile file(qssDir);
        file.open(QFile::ReadOnly);
        if (file.isOpen())
        {
            setStyleSheet(QString::fromUtf8(file.readAll()).toLatin1());
        }
        file.close();
}

void MusicDirDlg::updateListView(const QSet<QString> &dirSet)
{
    ui->listWidget->clear();
    foreach (const QString &value, dirSet)
    {
        if(!value.isEmpty()){
            QCheckBox* m_pChexkBox=new QCheckBox(ui->listWidget);
            m_pChexkBox->setCheckable(true);
            m_pChexkBox->setCheckState(Qt::Checked);
            m_pChexkBox->setText(value);
            m_pChexkBox->setObjectName("Check");
            connect(m_pChexkBox,&QCheckBox::clicked,this,&MusicDirDlg::test);
            QListWidgetItem *nitem = new QListWidgetItem(ui->listWidget);
            nitem->setSizeHint(QSize(0,30));
            ui->listWidget->addItem(nitem);
            ui->listWidget->setItemWidget(nitem,m_pChexkBox);
        }
    }
    update();
}

void MusicDirDlg::setDefautDir()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,QStringLiteral("PersonProject"), QStringLiteral("NeatMusic"), this);
    settings.setValue("Urls",QUrl(QStandardPaths::writableLocation(QStandardPaths::MusicLocation)));
    settings.setValue("Total",1);
}
void MusicDirDlg::saveAllExitDir()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,QStringLiteral("PersonProject"), QStringLiteral("NeatMusic"), this);
    QString urls;
    foreach (const QString &value, m_pMusicUrls)
    {
        urls=urls+value+";";
    }
    settings.setValue("Urls",urls);
    settings.setValue("Total",m_pMusicUrls.count());
    this->close();
}

void MusicDirDlg::loadStoredDir()
{
     QSettings settings(QSettings::IniFormat, QSettings::UserScope,QStringLiteral("PersonProject"), QStringLiteral("NeatMusic"), this);
    if(m_pMusicUrls.isEmpty()){

        QString urls=settings.value("Urls").toString();
        QStringList strList=urls.split(';');
        for (int i = 0; i < strList.size(); ++i){
             m_pMusicUrls.insert(strList.at(i).toLocal8Bit().constData());
        }
    }
    updateListView(m_pMusicUrls);
}

void MusicDirDlg::test()
{
    QPoint point=QCursor::pos();
    point=mapFromGlobal(point);
    QPoint temp;
    temp.setX(point.x());
    temp.setY(point.y()-40);//40=item.height+listWidget.Spacing
    QModelIndex listItem=ui->listWidget->indexAt(temp);
    QWidget *pu=ui->listWidget->indexWidget(listItem);
    QCheckBox *p=(QCheckBox *)pu;
    if(!p->isChecked()){
        m_pMusicUrls.remove(p->text());
    }
}

