#include "customlistview.h"
#include "QKeyEvent"


CustomListView::CustomListView(QWidget *parent) :
    QListView(parent)
{

}

void CustomListView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        event->accept();
        emit buttonDelPress();
    }
    if(event->key() == Qt::Key_Enter)
    {
        event->accept();
        emit buttonEnterPress();
    }
}
