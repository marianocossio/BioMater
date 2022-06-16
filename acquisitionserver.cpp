#include "acquisitionserver.h"

AcquisitionServer::AcquisitionServer(QObject *parent) : QObject(parent)
{
    connect(&thread, SIGNAL(dataReceived(QByteArray)), &dataConditioner, SLOT(newBytes(QByteArray)));
    connect(&thread, SIGNAL(portOpened()), this, SLOT(portOpenedSignalReceived()));
    connect(&thread, SIGNAL(portClosed()), this, SLOT(portClosedSignalReceived()));
    connect(&thread, SIGNAL(connectionError()), this, SLOT(connectionErrorSignalReceived()));
    connect(&dataConditioner, SIGNAL(dataReady(DataSet)), this, SLOT(dataReadySignalReceived(DataSet)));
}

AcquisitionServer::~AcquisitionServer()
{

}

bool AcquisitionServer::startPort(QString portName)
{
    return thread.startPort(portName);
}

void AcquisitionServer::stopPort()
{
    thread.stopPort();
}

bool AcquisitionServer::portIsActive()
{
    return thread.portIsActive();
}

void AcquisitionServer::write(QByteArray data)
{
    if (thread.portIsActive())
        thread.write(data);
}

QList<QString> AcquisitionServer::availablePorts()
{
    return thread.availablePorts();
}

void AcquisitionServer::setBaudRate(long baudRate)
{
    thread.setBaudRate(baudRate);
}

void AcquisitionServer::activateChannel(int channel)
{
    dataConditioner.activateChannel(channel);
}

void AcquisitionServer::deactivateChannel(int channel)
{
    dataConditioner.deactivateChannel(channel);
}

void AcquisitionServer::activateAllChannels()
{
    dataConditioner.activateAllChannels();
}

void AcquisitionServer::deactivateAllChannels()
{
    dataConditioner.deactivateAllChannels();
}

void AcquisitionServer::dataReadySignalReceived(DataSet data)
{
    emit dataReady(data);
}

void AcquisitionServer::portOpenedSignalReceived()
{
    emit portOpened();
}

void AcquisitionServer::portClosedSignalReceived()
{
    emit portClosed();
}

void AcquisitionServer::connectionErrorSignalReceived()
{
    emit portConnectionError();
}
