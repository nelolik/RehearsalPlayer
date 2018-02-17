#include "basepanel.h"
#include "QKeyEvent"
#include "QPainter"

BasePanel::BasePanel(QWidget *parent) : QFrame(parent)
{
    global_focus = false;
}


void BasePanel::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {
        emit keySpacePressed();
    }
}

void BasePanel::focusInEvent(QFocusEvent *event)
{
    global_focus = true;
    setFrameParams();
    QFrame::focusInEvent(event);
}

void BasePanel::focusOutEvent(QFocusEvent *event)
{
    global_focus = false;
    setFrameParams();
    QFrame::focusOutEvent(event);
}

void BasePanel::setFocusState(bool state)
{
    global_focus = state;
    setFrameParams();
}

void BasePanel::setFrameParams()
{
    if(global_focus)
    {
        setFrameShape(QFrame::Panel);
        setFrameShadow(QFrame::Sunken);
    }
    else {
        setFrameShape(QFrame::NoFrame);
    }
}
