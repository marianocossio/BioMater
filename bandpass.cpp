#include "bandpass.h"

BandPass::BandPass(QObject *parent) : Filter(parent)
{
    a = {1, -5.92515053832736, 15.3204206030155, -22.5781994350768, 20.7428341422466, -12.1648079665462, 4.44736315571761, -0.926717249431434, 0.0842682181183860};
    b = {1.20925491842172e-12, 0, -4.83701967368688e-12, 0, 7.25819404578942e-12, -3.55271367880050e-15, -4.83524331684748e-12, 1.11022302462516e-16, 1.20910226275583e-12};

    filteredSignal.resize(a.size(), 0);
    inputSignal.resize(a.size(), 0);
}

BandPass::~BandPass()
{

}

//        b[0] * x[n] + b[1] * x[n - 1] + b[2] * x[n - 2] + ... - a[1] * y[n - 1] - a[2] * y[n - 2] - ...
// y[n] = -----------------------------------------------------------------------------------------------
//                                                     a[0]

DataSet BandPass::filter(DataSet data, int channel)
{
    DataSet newData = data;

    inputSignal.pop_back();

    inputSignal.push_front(data.channelData(channel));

    double newChannelData = b[0] * inputSignal[0];

    for (unsigned k = 1; k < a.size(); k++)
        newChannelData = newChannelData + (b[k] * inputSignal[k]) - (a[k] * filteredSignal[k - 1]);

    newChannelData = newChannelData / a[0];

    filteredSignal.pop_back();

    filteredSignal.push_front(newChannelData);

    newChannelData = newChannelData * (-filterGain);

    newData.channelData(channel) = (short) newChannelData;

    return newData;
}

void BandPass::setGain(double gain)
{
    filterGain = gain;
}
