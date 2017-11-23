#include "playermainwindow.h"
#include "ui_playermainwindow.h"
#include "customlistview.h"
#include <QFileDialog>
#include <QSlider>
#include <QFileSystemModel>
#include <QTreeWidget>
#include <QTreeView>
#include <QItemSelectionModel>
#include <QDebug>
#include <QList>
#include <QMediaPlaylist>
#include <QModelIndex>

PlayerMainWindow::PlayerMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerMainWindow),
    player1_isPlaying(false),
    player2_isPlaying(false),
    goToNextTrack1(true),
    goToNextTrack2(true)
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
    ui->folders1_View->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->folders2_View->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->folders1_View->setRootIndex(rootPath1);
    ui->folders2_View->setRootIndex(rootPath2);
    ui->folders1_View->hideColumn(1);
    ui->folders1_View->hideColumn(2);
    ui->folders1_View->hideColumn(3);
    ui->folders2_View->hideColumn(1);
    ui->folders2_View->hideColumn(2);
    ui->folders2_View->hideColumn(3);


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
    playlistContainer1 = new QList<MediaItem>;
    playlistContainer2 = new QList<MediaItem>;
    playlistModel1 = new PlaylistModel(playlistContainer1, this);
    playlistModel2 = new PlaylistModel(playlistContainer2, this);
    ui->playlist1_View->setModel(playlistModel1);
    ui->playlist2_View->setModel(playlistModel2);

    soundExtentions = new QStringList;
    *soundExtentions << "wav" << "mp3" << "wma" << "aiff" << "flac";

    mediaPlaylist1 = new QMediaPlaylist;
    mediaPlaylist2 = new QMediaPlaylist;


    createConnections();


}

PlayerMainWindow::~PlayerMainWindow()
{
    delete ui;
    delete playlistContainer1;
    delete playlistContainer2;
    delete soundExtentions;
}

void PlayerMainWindow::createConnections()
{
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
    connect(this, SIGNAL(repaintRect(QRect)), ui->playlist1_View, SLOT(repaint(const QRect&)));
    connect(this, SIGNAL(playListDataChanged2()), ui->playlist2_View, SLOT(update()));
    connect(ui->playlist1_View, SIGNAL(buttonDelPress()), this, SLOT(deleteInPlaylist1()));
    connect(ui->playlist2_View, SIGNAL(buttonDelPress()), this, SLOT(deleteInPlaylist2()));

    connect(player1, SIGNAL(setDuration(int)), this, SLOT(onDurationChanged1(int)));
    connect(player2, SIGNAL(setDuration(int)), this, SLOT(onDurationChanged2(int)));
    connect(player1, SIGNAL(setPosition(int)), this, SLOT(onPositionChangged1(int)));
    connect(player2, SIGNAL(setPosition(int)), this, SLOT(onPositionChangged2(int)));
    connect(player1, SIGNAL(setLeftTime(int)), ui->left1lcdNumber, SLOT(display(int)));
    connect(player2, SIGNAL(setLeftTime(int)), ui->left2lcdNumber, SLOT(display(int)));
    connect(player1, SIGNAL(playbackStoped(QMediaPlayer::State)), this, SLOT(onPlaybackStoped1(QMediaPlayer::State)));
    connect(player2, SIGNAL(playbackStoped(QMediaPlayer::State)), this, SLOT(onPlaybackStoped2(QMediaPlayer::State)));

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
        ui->play1Button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    else
    {
        player1->pause();
        player1_isPlaying = false;
        ui->play1Button->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}

void PlayerMainWindow::play2clicked()
{
    if(!player2_isPlaying)
    {
        player2->play();
        player2_isPlaying = true;
        ui->play2Button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    else
    {
        player2->pause();
        player2_isPlaying = false;
        ui->play2Button->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}

void PlayerMainWindow::stop1clicked()
{
    player1->stop();
    player1_isPlaying = false;
    foreach (MediaItem item, *playlistContainer1) {
        if(item.isPlaying)
        {
            item.isPlaying = false;
        }
    }
    ui->play1Button->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    stopPressed1 = true;
}

void PlayerMainWindow::stop2clicked()
{
    player2->stop();
    player2_isPlaying = false;
    foreach (MediaItem item, *playlistContainer2) {
        if(item.isPlaying)
        {
            item.isPlaying = false;
        }
    }
    ui->play2Button->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    stopPressed2 = true;
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

//void PlayerMainWindow::onPlaybackStoped1()
//{

//}

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
    QModelIndexList selectedTracks = filesSelectionModel1->selectedRows(0);
    QModelIndex index;
    QList<MediaItem> validFiles;
    playlistModel1->clearContainer();
    foreach (index, selectedTracks) {
        QFileInfo fileInfo = model1->fileInfo(index);
        QString fileName = fileInfo.fileName();
        QString filePath = fileInfo.filePath();
        QString suffix = fileInfo.suffix();
        if(soundExtentions->contains(suffix, Qt::CaseInsensitive))
        {
            validFiles.append(MediaItem(fileName, filePath));
        }
    }
    playlistModel1->appendItems(validFiles);
    emit playListDataChanged1();
    if(playlistContainer1->size() > 0)
    {
        player1->setFileName(playlistContainer1->at(0).filePath);
        (*playlistContainer1)[0].isPlaying = true;
        player1->play();
        player1_isPlaying = true;
        ui->play1Button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }

}

void PlayerMainWindow::on_playSelected2_Button_clicked()
{
    QModelIndexList selectedTracks = filesSelectionModel2->selectedRows(0);
    QModelIndex index;
    QList<MediaItem> validFiles;
    playlistModel2->clearContainer();
    foreach (index, selectedTracks) {
        QFileInfo fileInfo = model2->fileInfo(index);
        QString fileName = fileInfo.fileName();
        QString filePath = fileInfo.filePath();
        QString suffix = fileInfo.suffix();
        if(soundExtentions->contains(suffix, Qt::CaseInsensitive))
        {
            validFiles.append(MediaItem(fileName, filePath));
        }
    }
    playlistModel2->appendItems(validFiles);
    emit playListDataChanged2();
    if(playlistContainer2->size() > 0)
    {
        player2->setFileName(playlistContainer2->at(0).filePath);
        (*playlistContainer2)[0].isPlaying = true;
        player2->play();
        player2_isPlaying = true;
        ui->play2Button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
}

void PlayerMainWindow::deleteInPlaylist1()
{
    QItemSelectionModel *selection = ui->playlist1_View->selectionModel();
    playlistModel1->onDelButton(selection);
    emit playListDataChanged1();
}

void PlayerMainWindow::deleteInPlaylist2()
{
    QItemSelectionModel *selection = ui->playlist2_View->selectionModel();
    playlistModel2->onDelButton(selection);
    emit playListDataChanged2();
}

void PlayerMainWindow::on_addToPlaylist1_Button_clicked()
{
    QModelIndexList selectedTracks = filesSelectionModel1->selectedRows(0);
    QModelIndex index;
    QList<MediaItem> validFiles;
    foreach (index, selectedTracks) {
        QFileInfo fileInfo = model1->fileInfo(index);
        QString fileName = fileInfo.fileName();
        QString filePath = fileInfo.filePath();
        QString suffix = fileInfo.suffix();
        if(soundExtentions->contains(suffix, Qt::CaseInsensitive))
        {
            validFiles.append(MediaItem(fileName, filePath));
        }
    }
    playlistModel1->appendItems(validFiles);
}

void PlayerMainWindow::on_addToPlaylist2_Button_clicked()
{
    QModelIndexList selectedTracks = filesSelectionModel2->selectedRows(0);
    QModelIndex index;
    QList<MediaItem> validFiles;
    foreach (index, selectedTracks) {
        QFileInfo fileInfo = model2->fileInfo(index);
        QString fileName = fileInfo.fileName();
        QString filePath = fileInfo.filePath();
        QString suffix = fileInfo.suffix();
        if(soundExtentions->contains(suffix, Qt::CaseInsensitive))
        {
            validFiles.append(MediaItem(fileName, filePath));
        }
    }
    playlistModel2->appendItems(validFiles);
}

int PlayerMainWindow::playingTrack(QList<MediaItem> *container)
{
    int i = 0;
    for(; i < container->size(); i++)
    {
        if(container->at(i).isPlaying)
        {
            break;
        }
    }
    return i;
}

void PlayerMainWindow::playNextTrack(QList<MediaItem> *playlist, SoundPlayer *player)
{
    int currentTarack = playingTrack(playlist);
    if(currentTarack < playlist->size())
    {
        (*playlist)[currentTarack].isPlaying = false;
        ++currentTarack;
        if(currentTarack < playlist->size())
        {
            player->setFileName(playlist->at(currentTarack).filePath);
            (*playlist)[currentTarack].isPlaying = true;
            player->play();


        }
    }
}

void PlayerMainWindow::onPlaybackStoped1(QMediaPlayer::State state)
{
    if(goToNextTrack1 && (state == QMediaPlayer::StoppedState) && !stopPressed1)
    {
        playNextTrack(playlistContainer1, player1);
        player1_isPlaying = true;
        ui->play1Button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    stopPressed1 = false;
}

void PlayerMainWindow::onPlaybackStoped2(QMediaPlayer::State state)
{
    if(goToNextTrack2 && (state == QMediaPlayer::StoppedState) && !stopPressed2)
    {
        playNextTrack(playlistContainer2, player2);
        player2_isPlaying = true;
        ui->play2Button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    stopPressed2 = false;
}
