#include "serverthread.h"

ServerThread::ServerThread(QObject *parent) : QThread(parent)
{
    abort = false;

    baudRate = 115200;

    connectionAttempts = 0;

    connect(&port, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(&port, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(errorOccurredReceived()));
}

ServerThread::~ServerThread()
{
    mutex.lock();

    abort = true;
    condition.wakeOne();

    mutex.unlock();

    wait();
}

bool ServerThread::startPort(QString portName)
{
    bool result;

    QMutexLocker locker(&mutex);

    port.setPortName(portName);
    port.setBaudRate(baudRate);
    port.open(QSerialPort::ReadWrite);

    result = port.isOpen();

    if (!isRunning())
        start(HighestPriority);
    else
        condition.wakeOne();

    if (result)
    {
        abort = false;

        connectionAttempts = 10;

        emit portOpened();
    }

    else
        emit connectionError();

    return result;
}

void ServerThread::stopPort()
{
    connectionAttempts = 0;

    mutex.lock();

    if (port.isOpen())
        port.close();

    abort = true;
    condition.wakeOne();

    mutex.unlock();

    exit();

    emit portClosed();
}

bool ServerThread::portIsActive()
{
    return port.isOpen();
}

void ServerThread::write(QByteArray data)
{
    mutex.lock();

    port.write(data);

    mutex.unlock();
}

QList<QString> ServerThread::availablePorts()
{
    QList<QString> availablePortNames;

    for (int index = 0; index < QSerialPortInfo::availablePorts().size(); index++)
        availablePortNames.push_back(QSerialPortInfo::availablePorts()[index].portName());

    return availablePortNames;
}

void ServerThread::setBaudRate(long baudRate)
{
    this->baudRate = baudRate;

    if (port.isOpen())
    {
        port.close();

        startPort(port.portName());
    }
}

void ServerThread::run()
{
    forever
    {
        if (abort)
            return;
    }
}

void ServerThread::readData()
{
    int bytesAvailable;

    mutex.lock();

    bytesAvailable = port.bytesAvailable();

    mutex.unlock();

    if (bytesAvailable)
    {
        QByteArray receivedBytes;

        mutex.lock();

        receivedBytes.append(port.read(bytesAvailable));

        mutex.unlock();

        if (receivedBytes.size() > 0)
            emit dataReceived(receivedBytes);
    }
}

void ServerThread::errorOccurredReceived()
{
    if (connectionAttempts < 10)
        connectionAttempts++;

    else
    {
        connectionAttempts = 0;

        emit connectionError();
    }
}
