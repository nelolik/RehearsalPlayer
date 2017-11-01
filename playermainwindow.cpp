#include "playermainwindow.h"
#include "ui_playermainwindow.h"
#include <QFileDialog>
#include <QSlider>
#include <QFileSystemModel>
#include <QTreeWidget>
#include <QTreeView>
#include <QItemSelectionModel>

PlayerMainWindow::PlayerMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerMainWindow),
    player1_isPlaying(false),
    player2_isPlaying(false)
{
    ui->setupUi(this);

    model1 = new QFileSystemModel(this);
    model2 = new QFileSystemModel(this);
    filesSelectionModel1 = new QItemSelectionModel;
    filesSelectionModel1->setModel(model1);
    filesSelectionModel2 = new QItemSelectionModel;
    filesSelectionModel2->setModel(model2);
    QModelIndex rootPath1 = model1->setRootPath(QString("/mnt/9A54ACA054AC809D/Users/home/Music/Music_D/"));
    QModelIndex rootPath2 = model2->setRootPath(QString("/mnt/9A54ACA054AC809D/Users/home/Music/Music_D/"));
    ui->folders1_View->setModel(model1);
    ui->folders2_View->setModel(model2);
    ui->folders1_View->setSelectionModel(filesSelectionModel1);
    ui->folders2_View->setSelectionModel(filesSelectionModel2);
    ui->folders1_View->setRootIndex(rootPath1);
    ui->folders2_View->setRootIndex(rootPath2);
    ui->folders1_View->hideColumn(1);
    ui->folders1_View->hideColumn(2);
    ui->folders1_View->hideColumn(3);
    ui->folders2_View->hideColumn(1);
    ui->folders2_View->hideColumn(2);
    ui->folders2_View->hideColumn(3);
    ui->folders1_View->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->folders2_View->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->back1Button->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->play1Button->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->stop1Button->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->forward1Button->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));

    ui->back2Button->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->play2Button->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->stop2Button->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->forward2Button->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->trackPosition1_Slider->setTickPosition(QSlider::TicksBothSides);
    ui->trackPosition1_Slider->setTickInterval(10);

    ui->trackPosition1_Slider->setMinimum(0);
    ui->trackPosition1_Slider->setMaximum(0);
    ui->trackPosition2_Slider->setMinimum(0);
    ui->trackPosition2_Slider->setMaximum(0);
    ui->trackPosition2_Slider->setTickPosition(QSlider::TicksBothSides);
    ui->trackPosition2_Slider->setTickInterval(10);


    player1 = new SoundPlayer(this);
    player2 = new SoundPlayer(this);

    connect(ui->play1Button, SIGNAL(clicked(bool)), this, SLOT(play1clicked()));
    connect(ui->play2Button, SIGNAL(clicked(bool)), this, SLOT(play2clicked()));
    connect(ui->stop1Button, SIGNAL(clicked(bool)), this, SLOT(stop1clicked()));
    connect(ui->stop2Button, SIGNAL(clicked(bool)), this, SLOT(stop2clicked()));
    connect(ui->openFile1Button, SIGNAL(clicked(bool)), this, SLOT(openFile1()));
    connect(ui->openFile2Button, SIGNAL(clicked(bool)), this, SLOT(openFile2()));
    connect(ui->trackPosition1_Slider, SIGNAL(sliderMoved(int)), this, SLOT(onSliderMoved1(int)));
    connect(ui->trackPosition2_Slider, SIGNAL(sliderMoved(int)), this, SLOT(onSliderMoved2(int)));
    connect(ui->trackPosition1_Slider, SIGNAL(actionTriggered(int)), this, SLOT(onSliderClicked1(int)));
    connect(ui->trackPosition2_Slider, SIGNAL(actionTriggered(int)), this, SLOT(onSliderClicked2(int)));
    connect(ui->back1Button, SIGNAL(clicked(bool)), this, SLOT(onBackButoonClicked1()));
    connect(ui->back2Button, SIGNAL(clicked(bool)), this, SLOT(onBackButoonClicked2()));
    connect(ui->forward1Button, SIGNAL(clicked(bool)), this, SLOT(onForwardButtonClicked1()));
    connect(ui->forward2Button, SIGNAL(clicked(bool)), this, SLOT(onForwardButtonClicked2()));

    connect(player1, SIGNAL(setDuration(int)), this, SLOT(onDurationChanged1(int)));
    connect(player2, SIGNAL(setDuration(int)), this, SLOT(onDurationChanged2(int)));
    connect(player1, SIGNAL(setPosition(int)), this, SLOT(onPositionChangged1(int)));
    connect(player2, SIGNAL(setPosition(int)), this, SLOT(onPositionChangged2(int)));
    connect(player1, SIGNAL(setLeftTime(int)), ui->left1lcdNumber, SLOT(display(int)));
    connect(player2, SIGNAL(setLeftTime(int)), ui->left2lcdNumber, SLOT(display(int)));


}

PlayerMainWindow::~PlayerMainWindow()
{
    delete ui;
}


void PlayerMainWindow::openFile1()
{
    QString strFile = QFileDialog::getOpenFileName(this, "Open File");
    if(!strFile.isEmpty())
    {
        player1->setFileName(strFile);
    }
}

void PlayerMainWindow::openFile2()
{
    QString strFile = QFileDialog::getOpenFileName(this, "Open File");
    if(!strFile.isEmpty())
    {
        player2->setFileName(strFile);
    }
}

void PlayerMainWindow::play1clicked()
{
    if(!player1_isPlaying)
    {
        player1->play();
        player1_isPlaying = true;
    }
    else
    {
        player1->pause();
        player1_isPlaying = false;
    }
}

void PlayerMainWindow::play2clicked()
{
    if(!player2_isPlaying)
    {
        player2->play();
        player2_isPlaying = true;
    }
    else
    {
        player2->pause();
        player2_isPlaying = false;
    }
}

void PlayerMainWindow::stop1clicked()
{
    player1->stop();
    player1_isPlaying = false;
}

void PlayerMainWindow::stop2clicked()
{
    player2->stop();
    player2_isPlaying = false;
}

void PlayerMainWindow::onDurationChanged1(int time)
{
    ui->left1lcdNumber->display(time);
    ui->trackPosition1_Slider->setMaximum(time);
    if(time == 0)
    {
        player1_isPlaying = false;
    }
}

void PlayerMainWindow::onDurationChanged2(int time)
{
    ui->left2lcdNumber->display(time);
    ui->trackPosition2_Slider->setMaximum(time);
    if(time == 0)
    {
        player2_isPlaying = false;
    }
}

void PlayerMainWindow::onPositionChangged1(int time)
{
    ui->elepsed1lcdNumber->display(time);
    ui->trackPosition1_Slider->setValue(time);
}

void PlayerMainWindow::onPositionChangged2(int time)
{
    ui->elepsed2lcdNumber->display(time);
    ui->trackPosition2_Slider->setValue(time);
}

void PlayerMainWindow::onSliderMoved1(int time)
{
    player1->setNewPosition(time);
}

void PlayerMainWindow::onSliderMoved2(int time)
{
    player2->setNewPosition(time);
}

void PlayerMainWindow::onSliderClicked1(int action)
{
    int position = ui->trackPosition1_Slider->value();
    switch (action) {
    case QAbstractSlider::SliderSingleStepAdd:
        player1->setNewPosition(position + ADD_SINGLE_STEP);
        break;
    case QAbstractSlider::SliderSingleStepSub:
        player1->setNewPosition(position - ADD_SINGLE_STEP);
        break;
    case QAbstractSlider::SliderPageStepAdd:
        player1->setNewPosition(position + ADD_PAGE_STEP);
        break;
    case QAbstractSlider::SliderPageStepSub:
        player1->setNewPosition(position - ADD_PAGE_STEP);
        break;
    }
}

void PlayerMainWindow::onSliderClicked2(int action)
{
    int position = ui->trackPosition2_Slider->value();
    switch (action) {
    case QAbstractSlider::SliderSingleStepAdd:
        player2->setNewPosition(position + ADD_SINGLE_STEP);
        break;
    case QAbstractSlider::SliderSingleStepSub:
        player2->setNewPosition(position - ADD_SINGLE_STEP);
        break;
    case QAbstractSlider::SliderPageStepAdd:
        player2->setNewPosition(position + ADD_PAGE_STEP);
        break;
    case QAbstractSlider::SliderPageStepSub:
        player2->setNewPosition(position - ADD_PAGE_STEP);
        break;
    }
}

void PlayerMainWindow::onBackButoonClicked1()
{
    int position = ui->trackPosition1_Slider->value();
    player1->setNewPosition(position - ADD_SINGLE_STEP);
}

void PlayerMainWindow::onBackButoonClicked2()
{
    int position = ui->trackPosition2_Slider->value();
    player2->setNewPosition(position - ADD_SINGLE_STEP);
}

void PlayerMainWindow::onForwardButtonClicked1()
{
    int position = ui->trackPosition1_Slider->value();
    player1->setNewPosition(position + ADD_SINGLE_STEP);
}

void PlayerMainWindow::onForwardButtonClicked2()
{
    int position = ui->trackPosition2_Slider->value();
    player2->setNewPosition(position + ADD_SINGLE_STEP);
}

void PlayerMainWindow::on_playSelected1_Button_clicked()
{

}
