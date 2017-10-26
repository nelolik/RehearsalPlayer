#ifndef PLAYERMAINWINDOW_H
#define PLAYERMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class PlayerMainWindow;
}

class PlayerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerMainWindow(QWidget *parent = 0);
    ~PlayerMainWindow();

private:
    Ui::PlayerMainWindow *ui;
};

#endif // PLAYERMAINWINDOW_H
