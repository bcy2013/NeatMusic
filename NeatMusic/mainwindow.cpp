#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"musicviewdelegrate.h"
#include"listviewitemmodel.h"
#include<QStandardItemModel>
#include"listviewitemdate.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_MusicView(Q_NULLPTR)
{
    ui->setupUi(this);
    setModelView();
}

MainWindow::~MainWindow()
{
    delete ui;
}
#include<QDebug>
void MainWindow::setModelView()
{
    m_MusicView=static_cast<MusicView *>(ui->listView);
    m_MusicView->setItemDelegate(new MusicViewDelegrate(m_MusicView));
    ListViewItemModel *model=new ListViewItemModel(this);
    ListViewItemDate *data=new ListViewItemDate();
    data->setItemIcon(QPixmap(":/Resources/btnSearch1.png"));
    data->setItemContent(QStringLiteral("搜索"));
    model->addItem(data);
    data=new ListViewItemDate();
    data->setItemIcon(QPixmap(":/Resources/btnmusic2.png"));
    data->setItemContent(QStringLiteral("发现音乐"));
    model->addItem(data);
    data=new ListViewItemDate();
    data->setItemIcon(QPixmap(":/Resources/btnmv.png"));
    data->setItemContent("MV");
    model->addItem(data);
    data=new ListViewItemDate();
    data->setItemIcon(QPixmap(":/Resources/btnSearch1.png"));
    data->setItemContent(QStringLiteral("朋友"));
    model->addItem(data);
    m_MusicView->setModel(model);
    m_MusicView->setCurrentIndex(model->index(1));
}
