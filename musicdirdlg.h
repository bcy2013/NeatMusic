#ifndef MUSICDIRDLG_H
#define MUSICDIRDLG_H

#include <QDialog>
#include<QSet>
#include<QCheckBox>
#include<QListWidgetItem>
namespace Ui {
class MusicDirDlg;
}

class MusicDirDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MusicDirDlg(QWidget *parent = 0);
    ~MusicDirDlg();

    void initDialog();
    void initConnections();
    void loadStyleSheet(const QString& qssDir);
    void updateListView(const QSet<QString> &dirSet);
    void setDefautDir();
public slots:
    void saveAllExitDir();
    void loadStoredDir();
    void test();


private:
    Ui::MusicDirDlg *ui;
    QSet< QString > m_pMusicUrls;
};

#endif // MUSICDIRDLG_H
