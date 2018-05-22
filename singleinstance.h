#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include <QObject>
#include<QLocalServer>
#include<QLocalSocket>
class SingleInstance : public QObject
{
    Q_OBJECT
public:
    explicit SingleInstance(QObject *parent = nullptr);

    void listen(const QString& name);
    bool hasPrevious(QString name);

signals:
   void newInstance();
public slots:
private:
    QLocalServer m_sever;
};

#endif // SINGLEINSTANCE_H
