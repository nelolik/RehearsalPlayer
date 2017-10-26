#include "playermainwindow.h"
#include "ui_playermainwindow.h"

PlayerMainWindow::PlayerMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerMainWindow)
{
    ui->setupUi(this);
}

PlayerMainWindow::~PlayerMainWindow()
{
    delete ui;
}
