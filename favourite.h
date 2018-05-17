#ifndef FAVOURITE_H
#define FAVOURITE_H

#include <QWidget>
#include<QSql>
#include<QSqlRelationalTableModel>
namespace Ui {
class Favourite;
}

class Favourite : public QWidget
{
    Q_OBJECT

public:
    explicit Favourite(QWidget *parent = 0);
    ~Favourite();
public slots:
    void updateView();

private:
    Ui::Favourite *ui;
    QSqlRelationalTableModel *model;
};

#endif // FAVOURITE_H
