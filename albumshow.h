#ifndef ALBUMSHOW_H
#define ALBUMSHOW_H

#include <QObject>
#include<QGraphicsItem>
class AlbumShow : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit AlbumShow(QObject *parent = nullptr);

signals:

public slots:

    // QGraphicsItem interface
public:
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
private:
    QPixmap albumBackGround;
    QPixmap albumPicture;
    qreal angle;
    // QObject interface
protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
};

#endif // ALBUMSHOW_H
