#include "system.h"

System::System(QObject *parent) : QObject(parent)
{
    graph.setWindowTitle("Gráfica");

    filter = new BandPass;
    filter->setGain(2e9);

    filterEnabled = false;
    recording = false;

    playAudioDelay.setSingleShot(true);

    connect(&acquisitionServer, SIGNAL(dataReady(DataSet)), this, SLOT(receiveData(DataSet)));
    connect(&acquisitionServer, SIGNAL(portConnectionError()), this, SLOT(portConnectionErrorSignalReceived()));
    connect(&playAudioDelay, SIGNAL(timeout()), this, SLOT(startAudio()));
}

System::~System()
{
    delete filter;
}

bool System::receivingData()
{
    return acquisitionServer.portIsActive();
}

QList<QString> System::availablePorts()
{
    return acquisitionServer.availablePorts();
}

bool System::start(QString portName, int baudRate)
{
    acquisitionServer.setBaudRate(baudRate);

    bool result = acquisitionServer.startPort(portName);

    if (result)
    {
        graph.show();
        graph.setCursor(Qt::BlankCursor);

        playPhonogram(true);
    }

    return result;
}

void System::stop()
{
    if (acquisitionServer.portIsActive())
        acquisitionServer.stopPort();

    if (graph.isVisible())
        graph.close();

    audioPlayer.stopPlayer();
}

void System::toggleGraphVisibility()
{
    if (graph.isVisible())
        graph.hide();
    else
        graph.show();
}

void System::playPhonogram(bool play)
{
    if (play)
    {
        audioPlayer.startPlayer();
        playAudioDelay.start(100);
    }

    else
        audioPlayer.stopPlayer();
}

void System::setPhonogramVolume(int volume)
{
    audioPlayer.setVolume(volume);
}

void System::enableFilter(bool enable)
{
    filterEnabled = enable;
}

void System::startRecording()
{
    recording = true;
}

bool System::isRecording()
{
    return recording;
}

void System::stopRecording()
{
    recording = false;

    bool graphIsVisible = graph.isVisible();

    if (graphIsVisible)
        toggleGraphVisibility();

    QString recordFileName = QFileDialog::getSaveFileName(nullptr, "Guardar Registro", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QDir::separator() + "nuevo.dat", "*.dat");

    if (!recordFileName.isEmpty())
    {
        recordFile.setFileName(recordFileName);

        recordFile.open(QFile::WriteOnly);

        if (recordFile.isOpen())
        {
            for (int bufferIndex = 0; bufferIndex < recordBuffer.size(); bufferIndex++)
                for (int channelIndex = 0; channelIndex < recordBuffer[bufferIndex].getDataType(); channelIndex++)
                    recordFile.write((char*) &recordBuffer[bufferIndex].channelData(channelIndex), sizeof(recordBuffer[bufferIndex].channelData(channelIndex)));

            recordFile.close();
        }
    }

    recordBuffer.clear();

    if (graphIsVisible)
        toggleGraphVisibility();
}

void System::receiveData(DataSet data)
{
    data.deactivateAllChannels();
    data.activateChannel(0);
    data.channelGain(0) = 1;
    data.channelName(0) = "Fonograma";

    if (filterEnabled)
        data = filter->filter(data, 0);

    if (isRecording())
        recordBuffer.push_back(data);

    graph.addData(data * 0.5);

    if (audioPlayer.isPlaying())
        audioPlayer.addData(data);
}

void System::portConnectionErrorSignalReceived()
{
    emit portConnectionError();
}

void System::startAudio()
{
    audioPlayer.play();
}
