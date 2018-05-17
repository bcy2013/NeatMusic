#include "musicinfoview.h"
#include"musicinfolistdelegrate.h"
#include <QPaintEvent>
#include<QTimer>
#include <QAbstractItemView>
MusicInfoView::MusicInfoView(QWidget *parent) : QListView(parent)
  ,m_isMousePressed(false)
{
     this->setAttribute(Qt::WA_MacShowFocusRect, 0);
     QTimer::singleShot(0, this, SLOT(init()));
}

MusicInfoView::~MusicInfoView()
{

}

void MusicInfoView::init()
{
    setMouseTracking(true);
    setUpdatesEnabled(true);
    viewport()->setAttribute(Qt::WA_Hover);
    setupSignalsSlots();
    setupStyleSheet();
}

void MusicInfoView::setupSignalsSlots()
{
    connect(selectionModel(), &QItemSelectionModel::currentRowChanged, [this]
            (const QModelIndex & current, const QModelIndex & previous){

        if(model() != Q_NULLPTR){
            if(current.row() < previous.row()){
                if(current.row() > 0){
                    QModelIndex prevIndex = model()->index(current.row()-1, 0);
                    viewport()->update(visualRect(prevIndex));
                }
            }

            if(current.row() > 1){
                QModelIndex prevPrevIndex = model()->index(current.row()-2, 0);
                viewport()->update(visualRect(prevPrevIndex));
            }
        }
    });

    // row was entered
    connect(this, &MusicInfoView::entered,[this](QModelIndex index){
        if(model() != Q_NULLPTR){
            if(index.row() > 1){
                QModelIndex prevPrevIndex = model()->index(index.row()-2, 0);
                viewport()->update(visualRect(prevPrevIndex));

                QModelIndex prevIndex = model()->index(index.row()-1, 0);
                viewport()->update(visualRect(prevIndex));

            }else if(index.row() > 0){
                QModelIndex prevIndex = model()->index(index.row()-1, 0);
                viewport()->update(visualRect(prevIndex));
            }

            MusicInfoListDelegrate* delegate = static_cast<MusicInfoListDelegrate *>(itemDelegate());
            if(delegate != Q_NULLPTR)
                delegate->setHoveredIndex(index);
        }
    });

    // viewport was entered
    connect(this, &MusicInfoView::viewportEntered,[this](){
        if(model() != Q_NULLPTR && model()->rowCount() > 1){
            MusicInfoListDelegrate* delegate = static_cast<MusicInfoListDelegrate *>(itemDelegate());
            if(delegate != Q_NULLPTR)
                delegate->setHoveredIndex(QModelIndex());

            QModelIndex lastIndex = model()->index(model()->rowCount()-2, 0);
            viewport()->update(visualRect(lastIndex));
        }
    });
}

void MusicInfoView::setupStyleSheet()
{
    QString ss = QString(
                         "QScrollBar {margin-right: 2px; background: transparent;} "
                         "QScrollBar:hover { background-color: rgb(217, 217, 217);}"
                         "QScrollBar:handle:vertical:hover { background: rgb(170, 170, 171); } "
                         "QScrollBar:handle:vertical:pressed { background: rgb(149, 149, 149);}"
                         "QScrollBar:vertical { border: none; width: 10px; border-radius: 4px;} "
                         "QScrollBar::handle:vertical { border-radius: 4px; background: rgb(188, 188, 188); min-height: 20px; }  "
                         "QScrollBar::add-line:vertical { height: 0px; subcontrol-position: bottom; subcontrol-origin: margin; }  "
                         "QScrollBar::sub-line:vertical { height: 0px; subcontrol-position: top; subcontrol-origin: margin; }"
                         );

    setStyleSheet(ss);
}


void MusicInfoView::mousePressEvent(QMouseEvent *event)
{
    m_isMousePressed=true;
    QListView::mousePressEvent(event);
}

void MusicInfoView::mouseReleaseEvent(QMouseEvent *event)
{
    m_isMousePressed=false;
    QListView::mouseReleaseEvent(event);
}

void MusicInfoView::mouseMoveEvent(QMouseEvent *event)
{
   if(!m_isMousePressed)
       QListView::mouseMoveEvent(event);
}

void MusicInfoView::paintEvent(QPaintEvent *event)
{
    MusicInfoListDelegrate *delegrate=new MusicInfoListDelegrate(itemDelegate());
    if(delegrate!=Q_NULLPTR)
        delegrate->setCurrentSelectedIndex(currentIndex());
    QListView::paintEvent(event);
}

bool MusicInfoView::viewportEvent(QEvent *e)
{
    if(model() != Q_NULLPTR){
        switch (e->type()) {
        case QEvent::Leave:{
            QPoint pt = mapFromGlobal(QCursor::pos());
            QModelIndex index = indexAt(QPoint(10, pt.y()));
            if(index.row() > 0){
                index = model()->index(index.row()-1, 0);
                MusicInfoListDelegrate* delegate = static_cast<MusicInfoListDelegrate*>(itemDelegate());
                if(delegate != Q_NULLPTR){
                    delegate->setHoveredIndex(QModelIndex());
                    viewport()->update(visualRect(index));
                }
            }
            break;
        }
        case QEvent::MouseButtonPress:{
            QPoint pt = mapFromGlobal(QCursor::pos());
            QModelIndex index = indexAt(QPoint(10, pt.y()));
            if(!index.isValid())
                emit viewportPressed();
            break;
        }
        default:
            break;
        }
    }

    return QListView::viewportEvent(e);
}

void MusicInfoView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QListView::mouseDoubleClickEvent(event);
}
