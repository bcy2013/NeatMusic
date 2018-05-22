#include "spectrumwidget.h"
#include"spectrograph.h"
#include<QScopedPointer>
#include<QVBoxLayout>
#include<QLabel>
#include<QFile>
#include"spectrum.h"
SpectrumWidget::SpectrumWidget(QWidget *parent) : QWidget(parent)
{
    initWidget();
    loadStyleSheet(":/qss/spectum.css");
}

void SpectrumWidget::initWidget()
{

    QVBoxLayout *mainLayout=new QVBoxLayout(this);
    m_pTitleLabel=new QLabel(QStringLiteral("音乐频谱"));
    m_pTitleLabel->setFixedHeight(30);
    m_pTitleLabel->setFont(QFont("Times", 10, QFont::Bold));
    m_pSpectrograph=new Spectrograph;
    m_pSpectrograph->setParams(SpectrumNumBands, SpectrumLowFreq, SpectrumHighFreq);
    QScopedPointer<QVBoxLayout> analiseLayout(new QVBoxLayout());
    analiseLayout->addWidget(m_pTitleLabel);
    analiseLayout->addWidget(m_pSpectrograph);
    mainLayout->addLayout(analiseLayout.data());
    analiseLayout.take();
    //mainLayout->addWidget(m_pTitleLabel);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_pSpectrograph);

}

void SpectrumWidget::loadStyleSheet(const QString &qssDir)
{
    QFile file(qssDir);
        file.open(QFile::ReadOnly);
        if (file.isOpen())
        {
            setStyleSheet(QString::fromUtf8(file.readAll()).toLatin1());
        }
        file.close();
}

void SpectrumWidget::spectrumChanged(qint64 position, qint64 length, const FrequencySpectrum &spectrum)
{
    m_pSpectrograph->spectrumChanged(spectrum);
}
