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

signals:
    void buttonDelPress();
    void buttonEnterPress();
};

#endif // CUSTOMLISTVIEW_H
