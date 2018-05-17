#ifndef MUSICINFOVIEW_H
#define MUSICINFOVIEW_H
#include<QListView>
class MusicInfoView : public QListView
{
    Q_OBJECT
public:
    explicit MusicInfoView(QWidget *parent = nullptr);
    ~MusicInfoView();

    void setupSignalsSlots();
    void setupStyleSheet();
signals:
    void viewportPressed();

public slots:
    void init();

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    // QAbstractScrollArea interface
protected:
    bool viewportEvent(QEvent *e) Q_DECL_OVERRIDE;
private:
    bool m_isScrollBarHidden;
    bool m_isMousePressed;

    // QWidget interface
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // MUSICINFOVIEW_H
