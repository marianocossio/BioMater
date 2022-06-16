#ifndef BANDPASS_H
#define BANDPASS_H

#include <vector>
#include <deque>

#include "filter.h"

using namespace std;

class BandPass : public Filter
{
public:
    BandPass(QObject *parent = 0);

    virtual ~BandPass();

    virtual DataSet filter(DataSet data, int channel);

    virtual void setGain(double gain);

private:
    vector <double> a;
    vector <double> b;
    deque <double> filteredSignal;
    deque <double> inputSignal;
};

#endif // BANDPASS_H
