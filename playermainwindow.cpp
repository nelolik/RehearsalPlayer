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
#include <QTime>
#include <qwindowdefs.h>

PlayerMainWindow::PlayerMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerMainWindow),
    player1_isPlaying(false), player2_isPlaying(false),
    goToNextTrack1(true), goToNextTrack2(true),
    stopButtonClicked(false), forwardButtonClicked(false), backButtonClicked(false)
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
//    QModelIndex rootPath1 = model1->setRootPath(QString(""));
//    QModelIndex rootPath2 = model2->setRootPath(QString(""));
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

    ui->volume1Slider->setValue(80);
    ui->volume1Slider->setTickPosition(QSlider::TicksBothSides);
    ui->volume1Slider->setTickInterval(10);
    ui->volume2Slider->setValue(80);
    ui->volume2Slider->setTickPosition(QSlider::TicksBothSides);
    ui->volume2Slider->setTickInterval(10);

    player1 = new SoundPlayer(this);
    player2 = new SoundPlayer(this);
    playlistContainer1 = new QList<MediaItem>;
    playlistContainer2 = new QList<MediaItem>;
    playlistModel1 = new PlaylistModel(playlistContainer1, this);
    playlistModel2 = new PlaylistModel(playlistContainer2, this);
    playlistSelectionModel1 = new QItemSelectionModel;
    playlistSelectionModel2 = new QItemSelectionModel;
    playlistSelectionModel1->setModel(playlistModel1);
    playlistSelectionModel2->setModel(playlistModel2);
    ui->playlist1_View->setModel(playlistModel1);
    ui->playlist2_View->setModel(playlistModel2);
    ui->playlist1_View->setSelectionModel(playlistSelectionModel1);
    ui->playlist2_View->setSelectionModel(playlistSelectionModel2);

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
    connect(ui->trackPosition1_Slider, SIGNAL(sliderMoved(int)), this, SLOT(onSliderMoved1(int)));
    connect(ui->trackPosition2_Slider, SIGNAL(sliderMoved(int)), this, SLOT(onSliderMoved2(int)));
    connect(ui->trackPosition1_Slider, SIGNAL(actionTriggered(int)), this, SLOT(onSliderClicked1(int)));
    connect(ui->trackPosition2_Slider, SIGNAL(actionTriggered(int)), this, SLOT(onSliderClicked2(int)));
    connect(ui->back1Button, SIGNAL(clicked(bool)), this, SLOT(onBackButoonClicked1()));
//    connect(ui->back1Button, SIGNAL(pressed()), this, SLOT(onBackButoonClicked1()));
    connect(ui->back2Button, SIGNAL(clicked(bool)), this, SLOT(onBackButoonClicked2()));
//    connect(ui->back2Button, SIGNAL(pressed()), this, SLOT(onBackButoonClicked2()));
    connect(ui->forward1Button, SIGNAL(clicked(bool)), this, SLOT(onForwardButtonClicked1()));
    connect(ui->forward2Button, SIGNAL(clicked(bool)), this, SLOT(onForwardButtonClicked2()));
    connect(this, SIGNAL(repaintRect(QRect)), ui->playlist1_View, SLOT(repaint(const QRect&)));
    connect(this, SIGNAL(playListDataChanged2()), ui->playlist2_View, SLOT(update()));
    connect(ui->playlist1_View, SIGNAL(buttonDelPress()), this, SLOT(deleteInPlaylist1()));
    connect(ui->playlist2_View, SIGNAL(buttonDelPress()), this, SLOT(deleteInPlaylist2()));
    connect(ui->playlist1_View, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onPlaylistDoubleclick1(QModelIndex)));
    connect(ui->playlist2_View, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onPlaylistDoubleclick2(QModelIndex)));
    connect(ui->playlist1_View, SIGNAL(buttonEnterPress()), this, SLOT(playSelectedTrack1()));
    connect(ui->playlist2_View, SIGNAL(buttonEnterPress()), this, SLOT(playSelectedTrack2()));
    connect(ui->leftPanelBaseWidget, SIGNAL(keySpacePressed()), this, SLOT(onKeySpace1()));
    connect(ui->rightPanelBaseWidget, SIGNAL(keySpacePressed()),this, SLOT(onKeySpace2()));
    connect(ui->playlist1_View, SIGNAL(sendFocusState(bool)), ui->leftPanelBaseWidget, SLOT(setFocusState(bool)));
    connect(ui->playlist2_View, SIGNAL(sendFocusState(bool)), ui->rightPanelBaseWidget, SLOT(setFocusState(bool)));

    connect(player1, SIGNAL(setDuration(int)), this, SLOT(onDurationChanged1(int)));
    connect(player2, SIGNAL(setDuration(int)), this, SLOT(onDurationChanged2(int)));
    connect(player1, SIGNAL(setPosition(int)), this, SLOT(onPositionChangged1(int)));
    connect(player2, SIGNAL(setPosition(int)), this, SLOT(onPositionChangged2(int)));
    connect(player1, SIGNAL(setLeftTime(QString)), ui->left1lcdNumber, SLOT(display(QString)));
    connect(player2, SIGNAL(setLeftTime(QString)), ui->left2lcdNumber, SLOT(display(QString)));
    connect(player1, SIGNAL(playbackStoped(QMediaPlayer::State)), this, SLOT(onPlaybackStoped1(QMediaPlayer::State)));
    connect(player2, SIGNAL(playbackStoped(QMediaPlayer::State)), this, SLOT(onPlaybackStoped2(QMediaPlayer::State)));
    connect(ui->volume1Slider, SIGNAL(valueChanged(int)), player1, SLOT(setVolume(int)));
    connect(ui->volume2Slider, SIGNAL(valueChanged(int)), player2, SLOT(setVolume(int)));

}



void PlayerMainWindow::play1clicked()
{
    if(!player1_isPlaying)
    {
        if(!player1->audioAvailable())
        {
            if(!setFirstTrackInPLaylist(1))
            {
                return;
            }
        }
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
        if(!player2->audioAvailable())
        {
            if(!setFirstTrackInPLaylist(2))
            {
                return;
            }
        }
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
    stopButtonClicked = true;
    player1->stop();
    player1_isPlaying = false;
//    foreach (MediaItem item, *playlistContainer1) {
//        if(item.isPlaying)
//        {
//            item.isPlaying = false;
//        }
//    }
    ui->play1Button->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}

void PlayerMainWindow::stop2clicked()
{
    stopButtonClicked = true;
    player2->stop();
    player2_isPlaying = false;
//    foreach (MediaItem item, *playlistContainer2) {
//        if(item.isPlaying)
//        {
//            item.isPlaying = false;
//        }
//    }
    ui->play2Button->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}

void PlayerMainWindow::onDurationChanged1(int time)
{
    QTime length(time / 3600, time % 3600 / 60, time % 60);
    ui->left1lcdNumber->display(length.toString("mm:ss"));
    ui->trackPosition1_Slider->setMaximum(time);
//    if(time == 0)
//    {
//        player1_isPlaying = false;
//    }
}

void PlayerMainWindow::onDurationChanged2(int time)
{
    QTime length(time / 3600, time % 3600 / 60, time % 60);
    ui->left2lcdNumber->display(length.toString("mm:ss"));
    ui->trackPosition2_Slider->setMaximum(time);
//    if(time == 0)
//    {
//        player2_isPlaying = false;
//    }
}

void PlayerMainWindow::onPositionChangged1(int time)
{
    QTime length(time / 3600, time % 3600 / 60, time % 60);
    ui->elepsed1lcdNumber->display(length.toString("mm:ss"));
    ui->trackPosition1_Slider->setValue(time);
}

void PlayerMainWindow::onPositionChangged2(int time)
{
    QTime length(time / 3600, time % 3600 / 60, time % 60);
    ui->elepsed2lcdNumber->display(length.toString("mm:ss"));
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
//    int position = ui->trackPosition1_Slider->value();
//    player1->setNewPosition(position - ADD_SINGLE_STEP);
    backButtonClicked = true;
    player1->stop();
}

void PlayerMainWindow::onBackButoonClicked2()
{
//    int position = ui->trackPosition2_Slider->value();
//    player2->setNewPosition(position - ADD_SINGLE_STEP);
    stopButtonClicked = true;
    player2->stop();
}

void PlayerMainWindow::onForwardButtonClicked1()
{
//    int position = ui->trackPosition1_Slider->value();
//    player1->setNewPosition(position + ADD_SINGLE_STEP);
    if(playingTrack(playlistContainer1) < playlistContainer1->size() - 1)
    {
        forwardButtonClicked = true;
        player1->stop();
    }
}

void PlayerMainWindow::onForwardButtonClicked2()
{
//    int position = ui->trackPosition2_Slider->value();
//    player2->setNewPosition(position + ADD_SINGLE_STEP);
    if(playingTrack(playlistContainer2) < playlistContainer2->size() - 1)
    {
        forwardButtonClicked = true;
        player2->stop();
    }
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
    if(playlistContainer2->size() > 0)
    {
        player2->setFileName(playlistContainer2->at(0).filePath);
        (*playlistContainer2)[0].isPlaying = true;
        player2->play();
        player2_isPlaying = true;
        ui->play2Button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
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
            return i;
        }
    }
    return i;
}

int PlayerMainWindow::selectPrevTrack(QList<MediaItem> *playlist, SoundPlayer *player)
{
    int currentTrack = playingTrack(playlist);
    if(currentTrack >= 0 && currentTrack < playlist->size())
    {
        (*playlist)[currentTrack].isPlaying = false;
        if(currentTrack > 0) {--currentTrack;}
        if(currentTrack >= 0)
        {
            player->setFileName(playlist->at(currentTrack).filePath);
            (*playlist)[currentTrack].isPlaying = true;
        }
        return currentTrack;
    }
    return -1;
}

int PlayerMainWindow::selectNextTrack(QList<MediaItem> *playlist, SoundPlayer *player)
{
    int currentTrack = playingTrack(playlist);
    if(currentTrack < playlist->size())
    {
        (*playlist)[currentTrack].isPlaying = false;
        ++currentTrack;
        if(currentTrack < playlist->size())
        {
            player->setFileName(playlist->at(currentTrack).filePath);
            (*playlist)[currentTrack].isPlaying = true;
            return currentTrack;
        }
    }
    return -1;
}

void PlayerMainWindow::onPlaybackStoped1(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::StoppedState)
    {
        if(backButtonClicked)
        {
            int selected_track = selectPrevTrack(playlistContainer1, player1);
            if(player1_isPlaying)
            {
                player1->play();
            }
            if(selected_track != -1)
            {
                playlistModel1->changedData(selected_track, selected_track + 1);
            }
            backButtonClicked = false;
        }
        else if((!stopButtonClicked && goToNextTrack1) || forwardButtonClicked)
        {
            //Трек доиграл до конца, переход на седующий. Или нажата кнопка NextTrack.
            int selected_track = selectNextTrack(playlistContainer1, player1);
            if(selected_track != -1)
            {
                playlistModel1->changedData(selected_track - 1, selected_track);
                if(player1_isPlaying)
                {
                    player1->play();
                }
            }
            else
            {
                ui->play1Button->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
                player1_isPlaying = false;
            }
            ui->play1Button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            stopButtonClicked = false;
            forwardButtonClicked = false;
        }
    }
}

void PlayerMainWindow::onPlaybackStoped2(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::StoppedState)
    {
        if(backButtonClicked)
        {
            int selected_track = selectPrevTrack(playlistContainer2, player2);
            if(player2_isPlaying)
            {
                player2->play();
            }
            if(selected_track != -1)
            {
                playlistModel2->changedData(selected_track, selected_track + 1);
            }
            backButtonClicked = false;
        }
        else if((!stopButtonClicked && goToNextTrack2) || forwardButtonClicked)
        {
            //Трек доиграл до конца, переход на седующий. Или нажата кнопка NextTrack.
            int selected_track = selectNextTrack(playlistContainer2, player2);
            if(selected_track != -1)
            {
                playlistModel2->changedData(selected_track - 1, selected_track);
                if(player2_isPlaying)
                {
                    player2->play();
                }
                ui->play2Button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            }
            else
            {
                ui->play2Button->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
                player2_isPlaying = false;
            }
            stopButtonClicked = false;
            forwardButtonClicked = false;
        }
    }
}

void PlayerMainWindow::deleteInPlaylist1()
{
    QItemSelectionModel *selection = ui->playlist1_View->selectionModel();
    playlistModel1->onDelButton(selection);
}

void PlayerMainWindow::deleteInPlaylist2()
{
    QItemSelectionModel *selection = ui->playlist2_View->selectionModel();
    playlistModel2->onDelButton(selection);
}

//void PlayerMainWindow::playSelectedTrack1()
//{
//    QItemSelectionModel *selection = ui->playlist1_View->selectionModel();
//    QModelIndexList selectedList = selection->selectedRows(0).at(0);
//    if(selectedList.size() > 0)
//    {
//        player1->stop();
//        QModelIndex currentTrack = selectedList.at(0);
//        player1->setFileName(currentTrack.filePath);
//        int prevTrack = playingTrack(playlistContainer1);
//        if(prevTrack < playlistContainer1->size())
//        {
//            playlistContainer1[prevTrack].isPlaying = false;
//        }
//    }
//}

//void PlayerMainWindow::playSelectedTrack2()
//{

//}

bool PlayerMainWindow::setFirstTrackInPLaylist(int player_number)
{
    switch (player_number) {
    case 1:
        if(playlistContainer1->size() > 0)
        {
            player1->setFileName(playlistContainer1->at(0).filePath);
            (*playlistContainer1)[0].isPlaying = true;
            return true;
        }
        else
        {
            return false;
        }
    case 2:
        if(playlistContainer2->size() > 0)
        {
            player2->setFileName(playlistContainer2->at(0).filePath);
            (*playlistContainer2)[0].isPlaying = true;
            return true;
        }
        else
        {
            return false;
        }
    default:

        return false;
    }
}

void PlayerMainWindow::onPlaylistDoubleclick1(QModelIndex clickedItem)
{
    if(clickedItem.isValid())
    {
        int currentTrack = playingTrack(playlistContainer1);
        if(currentTrack >= 0 && currentTrack < playlistContainer1->size())
        {
            (*playlistContainer1)[currentTrack].isPlaying = false;
            playlistModel1->changedData(currentTrack, currentTrack);
        }
        int clickedNumber = clickedItem.row();
        player1->stop();
        player1->setFileName(playlistContainer1->at(clickedNumber).filePath);
        (*playlistContainer1)[clickedNumber].isPlaying = true;
        player1->play();
        player1_isPlaying = true;
        ui->play1Button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        playlistModel1->changedData(clickedNumber, clickedNumber);
    }
}

void PlayerMainWindow::onPlaylistDoubleclick2(QModelIndex clickedItem)
{
    if(clickedItem.isValid())
    {
        int currentTrack = playingTrack(playlistContainer2);
        if(currentTrack >= 0 && currentTrack < playlistContainer2->size())
        {
            (*playlistContainer2)[currentTrack].isPlaying = false;
            playlistModel2->changedData(currentTrack, currentTrack);
        }
        int clickedNumber = clickedItem.row();
        player2->stop();
        player2->setFileName(playlistContainer2->at(clickedNumber).filePath);
        (*playlistContainer2)[clickedNumber].isPlaying = true;
        player2->play();
        player2_isPlaying = true;
        ui->play2Button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        playlistModel2->changedData(clickedNumber, clickedNumber);
    }
}

void PlayerMainWindow::onKeySpace1()
{
    play1clicked();
}

void PlayerMainWindow::onKeySpace2()
{
    play2clicked();
}

void PlayerMainWindow::playSelectedTrack1()
{
    QModelIndexList indexes = playlistSelectionModel1->selectedIndexes();
    if(indexes.size() == 1 && indexes.at(0).isValid())
    {
        int selected_track = indexes.at(0).row();
        int currentTrack = playingTrack(playlistContainer1);
        if(currentTrack >= 0 && currentTrack < playlistContainer1->size())
        {
            (*playlistContainer1)[currentTrack].isPlaying = false;
            playlistModel1->changedData(currentTrack, currentTrack);
        }
        player1->stop();
        player1->setFileName(playlistContainer1->at(selected_track).filePath);
        (*playlistContainer1)[selected_track].isPlaying = true;
        player1->play();
        player1_isPlaying = true;
        ui->play1Button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        playlistModel1->changedData(selected_track, selected_track);
    }
}

void PlayerMainWindow::playSelectedTrack2()
{
    QModelIndexList indexes = playlistSelectionModel2->selectedIndexes();
    if(indexes.size() == 1 && indexes.at(0).isValid())
    {
        int selected_track = indexes.at(0).row();
        int currentTrack = playingTrack(playlistContainer2);
        if(currentTrack >= 0 && currentTrack < playlistContainer2->size())
        {
            (*playlistContainer2)[currentTrack].isPlaying = false;
            playlistModel2->changedData(currentTrack, currentTrack);
        }
        player2->stop();
        player2->setFileName(playlistContainer2->at(selected_track).filePath);
        (*playlistContainer2)[selected_track].isPlaying = true;
        player2->play();
        player2_isPlaying = true;
        ui->play2Button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        playlistModel2->changedData(selected_track, selected_track);
    }
}
