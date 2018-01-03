#include "basepanel.h"
#include "QKeyEvent"

BasePanel::BasePanel(QWidget *parent) : QFrame(parent)
{

}


void BasePanel::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {
        emit keySpacePressed();
    }
}

void BasePanel::paintEvent(QPaintEvent *event)
{
    if(this->hasFocus())
    {
        setFrameShape(QFrame::Panel);
        setFrameShadow(QFrame::Sunken);
    }
    else {
        setFrameShape(QFrame::NoFrame);
    }
    QFrame::paintEvent(event);
}
