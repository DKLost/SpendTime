#include "musicplayer.h"

MusicPlayer::MusicPlayer(QObject *parent)
    : QMediaPlayer{parent}
{
    audioOutput = new QAudioOutput(this);
    setAudioOutput(audioOutput);
}

void MusicPlayer::playMusic()
{
    setSource(mp3FilePath);
    audioOutput->setVolume(0.33);
    play();
}

void MusicPlayer::stopMusic()
{
    stop();
}

QString MusicPlayer::getMp3FilePath() const
{
    return mp3FilePath;
}

void MusicPlayer::setMp3FilePath(const QString &newMp3FilePath)
{
    mp3FilePath = newMp3FilePath;
}
