#ifndef PLAYMUSICSHOW_H
#define PLAYMUSICSHOW_H

#include <QWidget>
#include<QPropertyAnimation>
#include<QGraphicsScene>
#include<QGraphicsView>
#include<QHBoxLayout>
class PlayMusicShow : public QGraphicsView
{
    Q_OBJECT

public:
    explicit PlayMusicShow(QWidget *parent = 0);
    ~PlayMusicShow();
    void toMax();
    void toMin();
    void initBackground();
    void initScene();

private:
    QGraphicsScene *m_pGraScene;
    QHBoxLayout *m_pHLayout;
    QImage gauss;

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;
};

#endif // PLAYMUSICSHOW_H
