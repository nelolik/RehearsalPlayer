#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QObject>
#include <QMediaPlayer>

class QMediaPlaylist;

class SoundPlayer : public QObject
{
    Q_OBJECT

protected:
    QMediaPlayer *m_mpl;
    QString m_fileName;
    int total_time;
    int elepsed_time;
    int left_time;
    bool playAfterAudioReady;


public:
    explicit SoundPlayer(QObject *parent = nullptr);
    void setFileName(QString filename);
    void play();
    void stop();
    void setNewPosition(int);

signals:
    void playStart();
    void playStop();
    void pause();
    void setPosition(int);
    void setLeftTime(int);
    void setDuration(int);
    void setPlaylist(QMediaPlaylist*);
    void playbackStoped(QMediaPlayer::State);

private slots:
    void changedPosition(qint64);
    void changedDuration(qint64);
    void onAudioAvailableChanged(bool available);
    void onStateChaged(QMediaPlayer::State);

public slots:
};

#endif // SOUNDPLAYER_H
