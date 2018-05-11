#ifndef MUSICVIEW_H
#define MUSICVIEW_H

#include <QListView>
class MusicView : public QListView
{
    Q_OBJECT
public:
    explicit MusicView(QWidget *parent = nullptr);
    ~MusicView();

signals:

public slots:

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
private:
    bool m_bIsMousePressed;
};

#endif // MUSICVIEW_H
