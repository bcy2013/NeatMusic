#ifndef UPDATEWINDOW_H
#define UPDATEWINDOW_H

#include <QWidget>

namespace Ui {
class updatewindow;
}
class QMouseEvent;
class QNetworkReply;
class QSimpleUpdater;
class QNetworkAccessManager;
class updatewindow : public QWidget
{
    Q_OBJECT

public:
    explicit updatewindow(QWidget *parent = 0);
    ~updatewindow();
    void setShowWindowDisable(const bool regardless);
    void startDownLoad(const QUrl& url);
public slots:
    void checkForUpdate(bool force);
private slots:
    void updateTitleLabel();
    void resetControl();
    void onCheckFinished(const QString &url);
    void startDownloadUpdates();
    void openDownloadFolder (const QString& file);
    void calculateSizes (qint64 received, qint64 total);
    void updateProgress (qint64 received, qint64 total);
    void calculateTimeRemaining (qint64 received, qint64 total);
    void onDownloadFinished();
     void openDownload(const QString &file);
signals:
    void dontShowUpdateWindowChanged(bool state);
private:
    Ui::updatewindow *ui;
    bool m_bCheckingForUpdates;
    bool m_bForce;
    bool m_dontShowUpdateWindow;

    QNetworkReply* m_reply;
    uint m_startTime;
    QSimpleUpdater* m_updater;
    QNetworkAccessManager* m_manager;

    int m_dPress_X;
    int m_dPress_Y;
    bool m_bIsPressed;

    QString m_fileName;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // UPDATEWINDOW_H
