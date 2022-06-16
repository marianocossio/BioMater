#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <QAudioOutput>
#include <QBuffer>

#include "dataset.h"

#define MAX_BUFFER_SIZE 10000

class AudioPlayer : public QThread
{
    Q_OBJECT

public:
    AudioPlayer();
    ~AudioPlayer();

    void startPlayer();
    void play();
    void stopPlayer();
    void addData(DataSet data);

    bool isPlaying();

public slots:
    void setVolume(int volume);

protected:
    void run() override;

private:
    QMutex mutex;
    QWaitCondition condition;

    QAudioOutput *audioOutput;
    QAudioFormat format;

    QByteArray phonogramBuffer;
    QBuffer phonogramBufferPlayer, phonogramBufferWriter;

    bool playing;
    bool abort;
};

#endif // AUDIOPLAYER_H
