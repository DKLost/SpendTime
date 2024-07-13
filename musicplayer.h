#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFile>

class MusicPlayer : public QMediaPlayer
{
    Q_OBJECT
public:
    explicit MusicPlayer(QObject *parent = nullptr);
    void playMusic();
    void stopMusic();

    QString getMp3FilePath() const;
    void setMp3FilePath(const QString &newMp3FilePath);

private:
    QString mp3FilePath;
    QFile *mp3File;
    QAudioOutput *audioOutput;

signals:
};

#endif // MUSICPLAYER_H
