#ifndef SPECTRUMWIDGET_H
#define SPECTRUMWIDGET_H

#include <QObject>
#include<QWidget>
class FrequencySpectrum;
class Spectrograph;

QT_BEGIN_NAMESPACE
class QAction;
class QAudioFormat;
class QLabel;
class QPushButton;
QT_END_NAMESPACE
class SpectrumWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SpectrumWidget(QWidget *parent = nullptr);
    void initWidget();
    void loadStyleSheet(const QString& qssDir);

signals:
public slots:
    void spectrumChanged(qint64 position, qint64 length,
                         const FrequencySpectrum &spectrum);
private:
    Spectrograph *m_pSpectrograph;
    QLabel       *m_pTitleLabel;
};

#endif // SPECTRUMWIDGET_H
