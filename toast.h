#ifndef TOAST_H
#define TOAST_H

#include <QWidget>
#include<QLabel>
#include<QVBoxLayout>
#include<QPropertyAnimation>
#include<QGraphicsOpacityEffect>
class Toast : public QWidget
{
    Q_OBJECT
public:
    explicit Toast(QWidget *parent = nullptr);

    void setText(const QString &text);
    void setPicture(const bool& pic);
    bool picture()const;
    void pop();

   // qreal opacity()const;
    //void setOpacity(const qreal& opacity);

signals:
    void modelChanged();
public slots:

private:
    QLabel *m_pLabel;
    QLabel *m_pPicture;
    QPropertyAnimation *m_aniProperty;
    QGraphicsOpacityEffect *m_graphicsOpacity;
    bool isHavePicture;
    //qreal m_fOpacity;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // TOAST_H
