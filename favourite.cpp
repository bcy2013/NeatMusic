#include "favourite.h"
#include "ui_favourite.h"
#include<QSqlDatabase>
#include<QDebug>
#include<QSqlError>
#include<QSqlQuery>
#include"favouritedelegrate.h"
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
    model->setFilter("favourite=1");
    if (!model->select()) {
        qDebug()<<model->lastError();
    }
    // biao tou
    model->setHeaderData(model->fieldIndex("id"), Qt::Horizontal, tr("ID"));
    model->setHeaderData(model->fieldIndex("title"), Qt::Horizontal, tr("Title"));
    model->setHeaderData(model->fieldIndex("artist"), Qt::Horizontal, tr("ArtistName"));
    model->setHeaderData(model->fieldIndex("album"), Qt::Horizontal, tr("AlbumName"));
    model->setHeaderData(model->fieldIndex("duration"), Qt::Horizontal, tr("Duration"));


    ui->tableView->setModel(model);
    ui->tableView->setMouseTracking(true);
    ui->tableView->setItemDelegate(new FavouriteDelegrate(ui->tableView));
    ui->tableView->setFocusPolicy(Qt::NoFocus);
    ui->tableView->setColumnHidden(model->fieldIndex("favourite"),true);
    ui->tableView->setColumnHidden(model->fieldIndex("size"),true);
     ui->tableView->setColumnHidden(model->fieldIndex("path"),true);
    ui->tableView->horizontalHeader()->resizeSection(0,50);
    ui->tableView->horizontalHeader()->resizeSection(1,304);
    ui->tableView->resizeRowsToContents();
    //ui->tableView->resizeColumnsToContents();
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

Favourite::~Favourite()
{
    delete ui;
}

void Favourite::updateView()
{
    model->setFilter("favourite=1");
    if (!model->select()) {
        qDebug()<<model->lastError();
    }
    ui->tableView->resizeRowsToContents();
   // ui->tableView->update();
}
