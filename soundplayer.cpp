#include "soundplayer.h"
#include <QMediaPlayer>

SoundPlayer::SoundPlayer(QObject *parent) : QObject(parent)
{
    total_time = 0;
    elepsed_time = 0;
    left_time = 0;

    m_mpl = new QMediaPlayer();
    m_mpl->setVolume(80);

    connect(this, SIGNAL(playStart()), m_mpl, SLOT(play()));
    connect(this, SIGNAL(playStop()), m_mpl, SLOT(stop()));
    connect(this, SIGNAL(pause()), m_mpl, SLOT(pause()));
    connect(m_mpl, SIGNAL(positionChanged(qint64)), this, SLOT(changedPosition(qint64)));
    connect(m_mpl, SIGNAL(durationChanged(qint64)), this, SLOT(changedDuration(qint64)));
}

void SoundPlayer::setFileName(QString filename)
{
    m_fileName = filename;
    if(!m_fileName.isEmpty() && !m_fileName.isNull())
    {
        m_mpl->setMedia(QUrl::fromLocalFile(m_fileName));
        total_time = m_mpl->duration() / 1000;
    }
}

void SoundPlayer::play()
{
    if(m_mpl->isAudioAvailable())
    {
        emit playStart();
    }
}

void SoundPlayer::stop()
{
    emit playStop();
}


void SoundPlayer::changedPosition(qint64 time)
{
    elepsed_time = (int)(time/1000);
    emit setPosition(elepsed_time);
    elepsed_time = total_time - elepsed_time;
    emit setLeftTime(elepsed_time);
}

void SoundPlayer::changedDuration(qint64 time)
{
    total_time = time / 1000;
    emit setDuration(total_time);
}

void SoundPlayer::setNewPosition(int time)
{
    if(time < 0)
    {
        time = 0;
    }
    if(time > total_time)
    {
        time = total_time;
    }
    m_mpl->setPosition(time * 1000);
}