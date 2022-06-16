#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>

#include "acquisitionserver.h"
#include "graph.h"
#include "audioplayer.h"
#include "bandpass.h"

class System : public QObject
{
    Q_OBJECT
public:
    explicit System(QObject *parent = 0);

    ~System();

    bool receivingData();

    QList < QString > availablePorts();

signals:
    void portConnectionError();

public slots:
    bool start(QString portName, int baudRate);
    void stop();

    void toggleGraphVisibility();
    void playPhonogram(bool play);
    void setPhonogramVolume(int volume);
    void enableFilter(bool enable);

    void startRecording();
    bool isRecording();
    void stopRecording();

private:
    Graph graph;
    AcquisitionServer acquisitionServer;
    AudioPlayer audioPlayer;
    Filter *filter;

    QTimer searchPortTicker, playAudioDelay;

    QFile recordFile;
    QVector<DataSet> recordBuffer;
    bool recording;

    bool filterEnabled;

private slots:
    void receiveData(DataSet data);
    void portConnectionErrorSignalReceived();
    void startAudio();
};

#endif // SYSTEM_H
