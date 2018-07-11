#include "updatewindow.h"
#include "ui_updatewindow.h"
#include<QTimer>
#include<QSimpleUpdater.h>
#include<QDir>
#include<QDesktopServices>
#include<QNetworkReply>
#include<QNetworkAccessManager>
#include<QDebug>
#include<QNetworkRequest>
static const QString UPDATE_URL("https://raw.githubusercontent.com/bcy2013/NeatMusic/master/UPDATES.json");
static const QDir DOWNLOAD_DIR(QDir::homePath()+ "/Downloads/");
updatewindow::updatewindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::updatewindow),
    m_bCheckingForUpdates(false),
    m_bForce(false),
    m_updater(QSimpleUpdater::getInstance()),
    m_dontShowUpdateWindow(false),
    m_bIsPressed(false),
    m_manager(new QNetworkAccessManager()),
    m_fileName("")
{
    m_reply=Q_NULLPTR;
    QFile file(":/qss/updateWindow.css");
    if(file.open(QIODevice::ReadOnly)){
        setStyleSheet(QString::fromUtf8(file.readAll()).toLatin1());
        file.close();
    }
    ui->setupUi(this);
    setWindowTitle(qApp->applicationName()+" "+QStringLiteral("更新程序"));
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowModality(Qt::ApplicationModal);

    updateTitleLabel();
    connect(ui->pBn_close,&QPushButton::clicked,this,&updatewindow::close);
    connect(ui->pBt_update,&QPushButton::clicked,this,&updatewindow::startDownloadUpdates);
    connect(m_updater,&QSimpleUpdater::checkingFinished,this,&updatewindow::onCheckFinished);
    connect(ui->checkBox,&QCheckBox::toggled,this,&updatewindow::dontShowUpdateWindowChanged);
}

updatewindow::~updatewindow()
{
    delete ui;
    delete m_reply;
    delete m_manager;
}

void updatewindow::setShowWindowDisable(const bool regardless)
{
    m_dontShowUpdateWindow=regardless;
    ui->checkBox->setChecked(m_dontShowUpdateWindow);
}

void updatewindow::startDownLoad(const QUrl &url)
{
    if(url.isEmpty()){
        QDesktopServices::openUrl(QUrl(m_updater->getOpenUrl(UPDATE_URL)));
        return;
    }

    if(m_reply){
       m_reply->abort();
       m_reply->deleteLater();
    }
    m_startTime=QDateTime::currentDateTime().toTime_t();
    QNetworkRequest netReq(url);
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    netReq.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
#endif
    m_reply=m_manager->get(netReq);

    m_fileName=m_updater->getDownloadUrl(UPDATE_URL).split("/").last();
    if(m_fileName.isEmpty()){
      m_fileName=QString("%1_Update_%2.bin")
              .arg(qApp->applicationName())
              .arg(qApp->applicationVersion());
    }
    if(!DOWNLOAD_DIR.exists()){
        DOWNLOAD_DIR.mkpath(".");
    }

    QFile::remove(DOWNLOAD_DIR.filePath(m_fileName));

    ui->widget_5->show();
    showNormal();

    connect(m_reply,&QNetworkReply::downloadProgress,this,&updatewindow::updateProgress);
    connect(m_reply,&QNetworkReply::finished,this,&updatewindow::onDownloadFinished);
}

void updatewindow::checkForUpdate(bool force)
{
    if(!m_updater->getUpdateAvailable(UPDATE_URL)){
        m_bCheckingForUpdates=true;
        m_bForce=force;

        m_updater->setNotifyOnFinish(UPDATE_URL,false);//false
        m_updater->setNotifyOnUpdate(UPDATE_URL,false);//false
        m_updater->setDownloaderEnabled(UPDATE_URL,false);//false
        m_updater->setUseCustomInstallProcedures(UPDATE_URL,true);
        m_updater->setModuleVersion(UPDATE_URL,qApp->applicationVersion());
        m_updater->checkForUpdates(UPDATE_URL);
    }
    if(force){
        if(!m_updater->getUpdateAvailable(UPDATE_URL)){
         ui->pBt_update->setEnabled(false);
         ui->titleLabel->setText(tr("Checking for updates...."));
        }
        resetControl();
        showNormal();
    }
}

void updatewindow::updateTitleLabel()
{
    if(m_bCheckingForUpdates){
        static int num=0;
        QString base=tr("Checking for updates");
        QString dot="";
        num++;
        dot.fill('.',num);
        ui->titleLabel->setText(base+dot);
        if(num==4)
            num=0;
    }
    QTimer::singleShot(500,this,SLOT(updateTitleLabel()));
}

void updatewindow::resetControl()
{
    ui->pBt_update->setEnabled(false);
    ui->currentVersion->setText(qApp->applicationVersion());
    ui->latestVersion->setText(m_updater->getLatestVersion(UPDATE_URL));
    if(m_updater->getUpdateAvailable(UPDATE_URL)){
        ui->titleLabel->setText(tr("A Newer Version is Available!"));
    }else{
        ui->titleLabel->setText(tr("You're up-to-date!"));
    }
    ui->textBrowser->setText(m_updater->getChangelog(UPDATE_URL));
    if(ui->textBrowser->toPlainText().isEmpty()){
        ui->textBrowser->setText("<p>No changelog found!!</p>");
    }

    ui->widget_5->hide();
    ui->progressBar->setValue(0);
    ui->downloadLabel->setText(tr("Downloading Updates")+ "...");
    ui->timeLabel->setText(tr("Time remaining")+ ": " + tr("unknown"));

    bool available = m_updater->getUpdateAvailable(UPDATE_URL);
    bool validOpenUrl = !m_updater->getOpenUrl(UPDATE_URL).isEmpty();
    bool validDownUrl = !m_updater->getDownloadUrl(UPDATE_URL).isEmpty();
    ui->pBt_update->setEnabled(available &&(validOpenUrl || validDownUrl));

//    /* Resize window */
//    bool showAgain = isVisible();
//    int height = minimumSizeHint().height();
//    int width = qMax(minimumSizeHint().width(),(int)(height * 1.2));
//    resize(QSize(width, height));

//    /* Re-show the window(if required)*/
//    if(showAgain){
//        showNormal();
//    }

}

void updatewindow::onCheckFinished(const QString &url)
{
    m_bCheckingForUpdates=false;
    if(m_updater->getUpdateAvailable(url)&&(UPDATE_URL == url)){
        if(!m_dontShowUpdateWindow){
            resetControl();
            showNormal();
        }
    }else if(m_bForce){
        m_bForce = false;
        resetControl();
        showNormal();
    }
}

void updatewindow::startDownloadUpdates()
{
    if(m_updater->getUpdateAvailable(UPDATE_URL)){
        ui->pBt_update->setEnabled(false);
        startDownLoad(m_updater->getDownloadUrl(UPDATE_URL));
    }
}
#include<QMessageBox>
void updatewindow::openDownloadFolder(const QString &file)
{
    /* Notify the user of the problem */
    QString extension = file.split(".").last();
    QMessageBox::information(this,
                             tr("Open Error"),
                             tr("It seems that your OS does not have an "
                                "application that can handle *.%1 files. "
                                "We'll open the downloads folder for you.")
                             .arg(extension), QMessageBox::Ok);

    /* Get the full path list of the downloaded file */
    QString native_path = QDir::toNativeSeparators(QDir(file).absolutePath());
    QStringList directories = native_path.split(QDir::separator());

    /* Remove file name from list to get the folder of the update file */
    directories.removeLast();
    QString path = directories.join (QDir::separator());

    /* Get valid URL and open it */
    QUrl url = QUrl::fromLocalFile(QDir(path).absolutePath());
    QDesktopServices::openUrl(url);
}

void updatewindow::calculateSizes(qint64 received, qint64 total)
{
    QString totalSize;
    QString receivedSize;

    /* Get total size string */
    if(total < 1024){
        totalSize = tr("%1 bytes").arg(total);
    }else if(total < (1024 * 1024)){
        totalSize = tr("%1 KB").arg(round(total / 1024));
    }else{
        totalSize = tr("%1 MB").arg(round(total / (1024 * 1024)));
    }

    /* Get received size string */
    if(received < 1024){
        receivedSize = tr("%1 bytes").arg(received);
    }else if(received < (1024 * 1024)){
        receivedSize = tr("%1 KB").arg(received / 1024);
    }else{
        receivedSize = tr("%1 MB").arg(received / (1024 * 1024));
    }

    /* Update the label text */
    ui->downloadLabel->setText(tr("Downloading updates")
                                 + " (" + receivedSize + " " + tr("of")
                                 + " " + totalSize + ")");
}

void updatewindow::updateProgress(qint64 received, qint64 total)
{
    if(total>0){
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(100);
        ui->progressBar->setValue((received*100)/total);

        calculateSizes(received,total);
        calculateTimeRemaining(received,total);
    }else{
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(0);
        ui->progressBar->setValue(-1);
        ui->downloadLabel->setText(tr("Downloading Updates")+ "...");
        ui->timeLabel->setText(QString("%1: %2")
                                 .arg(tr("Time Remaining"))
                                 .arg(tr("unknown")));
    }
}

void updatewindow::calculateTimeRemaining(qint64 received, qint64 total)
{
    uint difference = QDateTime::currentDateTime().toTime_t() - m_startTime;

    if(difference > 0){
        QString timeString;
        qreal timeRemaining = (total-received)/(received / difference);

        if(timeRemaining > 7200){
            timeRemaining /= 3600;
            int hours = int(timeRemaining + 0.5);

            if(hours > 1){
                timeString = tr("about %1 hours").arg(hours);
            }else{
                timeString = tr("about one hour");
            }
        }else if(timeRemaining > 60){
            timeRemaining /= 60;
            int minutes = int(timeRemaining + 0.5);

            if(minutes > 1){
                timeString = tr("%1 minutes").arg(minutes);
            }else{
                timeString = tr("1 minute");
            }
        }else if(timeRemaining <= 60){
            int seconds = int(timeRemaining + 0.5);

            if(seconds > 1){
                timeString = tr("%1 seconds").arg(seconds);
            }else{
                timeString = tr("1 second");
            }
        }

        ui->timeLabel->setText(tr("Time remaining")+ ": " + timeString);
    }
}

void updatewindow::onDownloadFinished()
{
    QString redirectedUrl = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();

    if(redirectedUrl.isEmpty()){

         QString filePath = DOWNLOAD_DIR.filePath(m_fileName);
         QFile file(filePath);
         if(file.open(QIODevice::WriteOnly | QIODevice::Append)){
             file.write(m_reply->readAll());
             file.close();
             qApp->processEvents();
         }
         openDownload(filePath);
    }else{
        startDownLoad(redirectedUrl);
    }
}

void updatewindow::openDownload(const QString &file)
{
    if(file.isEmpty()){
        return;
    }
    ui->downloadLabel->setText(tr("Download finished!"));
    ui->timeLabel->setText(tr("Opening downloaded file")+"...");
    bool openUrl = QDesktopServices::openUrl(QUrl::fromLocalFile(file));
    if(!openUrl){
        openDownloadFolder(file);
        ui->widget_5->hide();
        qApp->processEvents();
        showNormal();
        close();
    } else {
        qApp->quit();
    }
}


void updatewindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        if(event->pos().x()< width() - 5
                && event->pos().x()>5
                && event->pos().y()< height() - 5
                && event->pos().y()> 5){
            m_bIsPressed = true;
            m_dPress_X = event->x();
            m_dPress_Y = event->y();
        }
    }
    event->accept();
}

void updatewindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_bIsPressed=false;
    unsetCursor();
    event->accept();
}

void updatewindow::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bIsPressed){
        setCursor(Qt::ClosedHandCursor);
        int dx=event->globalX()-m_dPress_X;
        int dy=event->globalY()-m_dPress_Y;
        move(dx,dy);
    }
}
