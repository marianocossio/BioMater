#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QByteArray>
#include <QList>

class ServerThread : public QThread
{
    Q_OBJECT

public:
    ServerThread(QObject *parent = 0);
    ~ServerThread();

    bool startPort(QString portName);
    void stopPort();
    bool portIsActive();
    void write(QByteArray data);

    QList<QString> availablePorts();

signals:
    void dataReceived(QByteArray data);
    void portOpened();
    void portClosed();
    void connectionError();

public slots:
    void setBaudRate(long baudRate);

protected:
    void run() override;

private:
    QMutex mutex;
    QWaitCondition condition;

    QSerialPort port;

    bool abort;
    long baudRate;
    int connectionAttempts;

private slots:
    void readData();
    void errorOccurredReceived();
};

#endif // SERVERTHREAD_H
