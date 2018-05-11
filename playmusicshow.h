#ifndef PLAYMUSICSHOW_H
#define PLAYMUSICSHOW_H

#include <QWidget>
#include<QPropertyAnimation>
namespace Ui {
class PlayMusicShow;
}

class PlayMusicShow : public QWidget
{
    Q_OBJECT

public:
    explicit PlayMusicShow(QWidget *parent = 0);
    ~PlayMusicShow();
    void toMax();
    void toMin();


private:
    Ui::PlayMusicShow *ui;
};

#endif // PLAYMUSICSHOW_H
