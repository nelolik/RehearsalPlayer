#ifndef PLAYERMAINWINDOW_H
#define PLAYERMAINWINDOW_H

#include <QMainWindow>
#include "soundplayer.h"
#include "playlistmodel.h"
#include "mediaitem.h"

namespace Ui {
class PlayerMainWindow;
}
class QFileSystemModel;
class QItemSelectionModel;
class QStringList;
class QMediaPlaylist;
class QTimer;

#define ADD_SINGLE_STEP 2
#define ADD_PAGE_STEP 5

class PlayerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerMainWindow(QWidget *parent = 0);
    ~PlayerMainWindow();

private:
    Ui::PlayerMainWindow *ui;

    SoundPlayer *player1;
    SoundPlayer *player2;

    bool player1_isPlaying, player2_isPlaying;
    bool goToNextTrack1, goToNextTrack2;
    bool stopButtonClicked, forwardButtonClicked, backButtonClicked;
    bool stop2ndPanel, fadingStop;
    int fadingTime;
    QFileSystemModel *model1;
    QFileSystemModel *model2;
    QItemSelectionModel *filesSelectionModel1, *filesSelectionModel2;
    QItemSelectionModel *playlistSelectionModel1, *playlistSelectionModel2;
    QList<MediaItem> *playlistContainer1, *playlistContainer2;
    QStringList *soundExtentions;
    QMediaPlaylist *mediaPlaylist1, *mediaPlaylist2;
    PlaylistModel *playlistModel1, *playlistModel2;
    QTimer *timer1, *timer2;

    void createConnections();
    int playingTrack(QList<MediaItem>*);
    int selectNextTrack(QList<MediaItem>* playlist, SoundPlayer *player);
    int selectPrevTrack(QList<MediaItem>* playlist, SoundPlayer *player);
    bool setFirstTrackInPLaylist(int player_number);
    void setFadingVolume(SoundPlayer *player, QTimer *timer);

signals:
    void setMediaPlaylist1(QMediaPlaylist*);
    void setMediaPlaylist2(QMediaPlaylist*);

public slots:


private slots:
    void play1clicked();
    void play2clicked();
    void stop1clicked();
    void stop2clicked();
    void onDurationChanged1(int);
    void onDurationChanged2(int);
    void onPositionChangged1(int);
    void onPositionChangged2(int);
    void onPlaybackStoped1(QMediaPlayer::State);
    void onPlaybackStoped2(QMediaPlayer::State);
    void onSliderMoved1(int);
    void onSliderMoved2(int);
    void onSliderClicked1(int);
    void onSliderClicked2(int);
    void onBackButoonClicked1();
    void onBackButoonClicked2();
    void onForwardButtonClicked1();
    void onForwardButtonClicked2();
    void on_playSelected1_Button_clicked();
    void on_playSelected2_Button_clicked();
    void on_addToPlaylist1_Button_clicked();
    void on_addToPlaylist2_Button_clicked();
    void deleteInPlaylist1();
    void deleteInPlaylist2();
    void onPlaylistDoubleclick1(QModelIndex);
    void onPlaylistDoubleclick2(QModelIndex);
    void onKeySpace1();
    void onKeySpace2();
    void playSelectedTrack1();
    void playSelectedTrack2();
    void onActionSettings();
    void onTimerTimeout1();
    void onTimerTimeout2();
};

#endif // PLAYERMAINWINDOW_H
