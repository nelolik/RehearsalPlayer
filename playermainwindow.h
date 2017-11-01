#ifndef PLAYERMAINWINDOW_H
#define PLAYERMAINWINDOW_H

#include <QMainWindow>
#include "soundplayer.h"

namespace Ui {
class PlayerMainWindow;
}
class QFileSystemModel;
class QItemSelectionModel;

#define ADD_SINGLE_STEP 2
#define ADD_PAGE_STEP 5

class PlayerMainWindow : public QMainWindow
{
    Q_OBJECT

    SoundPlayer *player1;
    SoundPlayer *player2;

    bool player1_isPlaying;
    bool player2_isPlaying;
    QFileSystemModel *model1;
    QFileSystemModel *model2;
    QItemSelectionModel *filesSelectionModel1, *filesSelectionModel2;

public:
    explicit PlayerMainWindow(QWidget *parent = 0);
    ~PlayerMainWindow();

private:
    Ui::PlayerMainWindow *ui;

public slots:
    void openFile1();
    void openFile2();

private slots:
    void play1clicked();
    void play2clicked();
    void stop1clicked();
    void stop2clicked();
    void onDurationChanged1(int);
    void onDurationChanged2(int);
    void onPositionChangged1(int);
    void onPositionChangged2(int);
    void onSliderMoved1(int);
    void onSliderMoved2(int);
    void onSliderClicked1(int);
    void onSliderClicked2(int);
    void onBackButoonClicked1();
    void onBackButoonClicked2();
    void onForwardButtonClicked1();
    void onForwardButtonClicked2();
    void on_playSelected1_Button_clicked();
};

#endif // PLAYERMAINWINDOW_H
