#include "audioplayer.h"

AudioPlayer::AudioPlayer()
{
    abort = false;
    playing = false;

    format.setSampleRate(1000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    audioOutput = new QAudioOutput(format);

    phonogramBufferPlayer.setBuffer(&phonogramBuffer);
    phonogramBufferWriter.setBuffer(&phonogramBuffer);
}

AudioPlayer::~AudioPlayer()
{
    mutex.lock();

    delete audioOutput;

    abort = true;
    condition.wakeOne();

    mutex.unlock();

    wait();
}

void AudioPlayer::startPlayer()
{
    QMutexLocker locker(&mutex);

    if (!isRunning())
        start(LowestPriority);
    else
        condition.wakeOne();

    abort = false;

    phonogramBufferPlayer.open(QBuffer::ReadOnly);
    phonogramBufferWriter.open(QBuffer::WriteOnly);
}

void AudioPlayer::play()
{
    mutex.lock();

    playing = true;

    audioOutput->start((QIODevice*) &phonogramBufferPlayer);

    mutex.unlock();
}

void AudioPlayer::stopPlayer()
{
    mutex.lock();

    playing  = false;

    abort = true;
    condition.wakeOne();

    mutex.unlock();

    audioOutput->stop();
    phonogramBufferPlayer.close();
    phonogramBufferWriter.close();

    phonogramBuffer.clear();

    exit();
}

void AudioPlayer::addData(DataSet data)
{
    mutex.lock();

    phonogramBufferWriter.write((char*) &data.channelData(0), sizeof(short));

    if ((phonogramBufferWriter.pos() - phonogramBufferPlayer.pos()) < 10)
        phonogramBufferPlayer.seek(phonogramBufferPlayer.pos() - 10);

    mutex.unlock();
}

bool AudioPlayer::isPlaying()
{
    return playing;
}

void AudioPlayer::setVolume(int volume)
{
    mutex.lock();

    audioOutput->setVolume(volume / 100.0f);

    mutex.unlock();
}

void AudioPlayer::run()
{
    forever
    {
        if (abort)
            return;
    }
}
