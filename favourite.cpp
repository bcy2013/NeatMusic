#include "favourite.h"
#include "ui_favourite.h"
#include<QSqlDatabase>
#include<QDebug>
#include<QSqlError>
#include<QSqlQuery>
#include<favouritedelegrate.h>
Favourite::Favourite(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Favourite)
{
    ui->setupUi(this);

    QString path="C://Users//bcy20//AppData//Roaming//PersonProject//music.db";
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE","favouriteConnection");
    db.setDatabaseName(path);
    if(!db.open()){
        qDebug()<<"DB Open Failed!!";
    }else{
        qDebug()<<db.tables();
    }

    model=new QSqlRelationalTableModel(ui->tableView,db);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("all_music_info");
    if (!model->select()) {
        qDebug()<<model->lastError();
    }
    // biao tou
    model->setHeaderData(model->fieldIndex("id"), Qt::Horizontal, tr("ID"));
    model->setHeaderData(model->fieldIndex("title"), Qt::Horizontal, tr("Title"));
    model->setHeaderData(model->fieldIndex("artist"), Qt::Horizontal, tr("ArtistName"));
    model->setHeaderData(model->fieldIndex("album"), Qt::Horizontal, tr("AlbumName"));
    model->setHeaderData(model->fieldIndex("duration"), Qt::Horizontal, tr("Duration"));
    model->setHeaderData(model->fieldIndex("favourite"), Qt::Horizontal, tr("Faritevou"));
    model->setHeaderData(model->fieldIndex("size"), Qt::Horizontal, tr("MusicSize"));


    ui->tableView->setModel(model);
    ui->tableView->setItemDelegate(new FavouriteDelegrate(ui->tableView));
    ui->tableView->setFocusPolicy(Qt::NoFocus);
    ui->tableView->horizontalHeader()->resizeSection(0,50);
    //ui->tableView->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setColumnHidden(model->fieldIndex("favourite"),true);
    ui->tableView->setColumnHidden(model->fieldIndex("size"),true);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为时每次选择一行
}

Favourite::~Favourite()
{
    delete ui;
}
