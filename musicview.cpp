#include "musicview.h"
#include"musicviewdelegrate.h"
MusicView::MusicView(QWidget *parent) : QListView(parent)
  ,m_bIsMousePressed(false)
{

}

MusicView::~MusicView()
{

}


void MusicView::mousePressEvent(QMouseEvent *event)
{
    m_bIsMousePressed=true;
    QListView::mousePressEvent(event);
}

void MusicView::mouseReleaseEvent(QMouseEvent *event)
{
    m_bIsMousePressed=false;
    QListView::mouseReleaseEvent(event);
}

void MusicView::mouseMoveEvent(QMouseEvent *event)
{
    if(!m_bIsMousePressed)
        QListView::mouseMoveEvent(event);
}

void MusicView::paintEvent(QPaintEvent *event)
{
    MusicViewDelegrate* delegate = static_cast<MusicViewDelegrate*>(itemDelegate());
    if(delegate != Q_NULLPTR)
        delegate->setCurrentSelectedIndex(currentIndex());
    QListView::paintEvent(event);

}
