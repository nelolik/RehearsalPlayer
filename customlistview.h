#ifndef CUSTOMLISTVIEW_H
#define CUSTOMLISTVIEW_H

#include "QListView"

class CustomListView : public QListView
{
    Q_OBJECT
public:
    CustomListView(QWidget *parent = Q_NULLPTR);

protected:
    void keyPressEvent(QKeyEvent*) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event);

signals:
    void buttonDelPress();
    void buttonEnterPress();
    void sendFocusState(bool state);
};

#endif // CUSTOMLISTVIEW_H
