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
    else if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        event->accept();
        emit buttonEnterPress();
    }
    else
    {
        event->ignore();
    }
}
