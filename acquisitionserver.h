#ifndef ACQUISITIONSERVER_H
#define ACQUISITIONSERVER_H

#include <QObject>

#include "serverthread.h"
#include "dataconditioner.h"

using namespace std;

class AcquisitionServer : public QObject
{
    Q_OBJECT

public:

    explicit AcquisitionServer(QObject *parent = 0);
    virtual ~AcquisitionServer();

    virtual bool startPort(QString portName);
    virtual void stopPort();
    virtual bool portIsActive();
    virtual void write(QByteArray data);
    virtual QList<QString> availablePorts();

signals:
    void dataReady(DataSet data);
    void portOpened();
    void portClosed();
    void portConnectionError();

public slots:
    virtual void setBaudRate(long baudRate);
    virtual void activateChannel(int channel);
    virtual void deactivateChannel(int channel);
    virtual void activateAllChannels();
    virtual void deactivateAllChannels();

protected:

private:
    ServerThread thread;
    DataConditioner dataConditioner;

private slots:
    virtual void dataReadySignalReceived(DataSet data);
    virtual void portOpenedSignalReceived();
    virtual void portClosedSignalReceived();
    virtual void connectionErrorSignalReceived();
};

#endif // ACQUISITIONSERVER_H
