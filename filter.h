#ifndef FILTER_H
#define FILTER_H

#include <QObject>

#include <deque>

#include "dataset.h"

class Filter : public QObject
{
    Q_OBJECT

public:
    explicit Filter(QObject *parent = 0);

    virtual ~Filter();

    virtual DataSet filter(DataSet data, int channel) = 0;

    virtual void setGain(double gain) = 0;

signals:

public slots:

protected:
    int filterGain;
};

#endif // FILTER_H
