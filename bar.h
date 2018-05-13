#ifndef BAR_H
#define BAR_H

#include <QObject>
#include<QGraphicsItem>
class Bar : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Bar(QObject *parent = nullptr);

signals:

public slots:

    // QGraphicsItem interface
public:
    enum State{
        PlayState,
        PauseState
    };
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
private:
    QPixmap barImg;
    State m_State;

    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // BAR_H
