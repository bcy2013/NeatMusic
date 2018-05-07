#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"musicview.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setModelView();
private:
    Ui::MainWindow *ui;
    MusicView *m_MusicView;
    MusicView *m_MusicView_Next;
};

#endif // MAINWINDOW_H
