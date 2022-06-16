#ifndef DATASET_H
#define DATASET_H

#include <vector>
#include <string>

#include <dataset.h>

using namespace std;

class DataSet
{
public:
    enum DataType{
        DataSet_8 = 8,
        DataSet_16 = 16
    };

    DataSet(DataType type = DataSet_8, DataSet *parent = 0);
    virtual ~DataSet();

    virtual void setDataOrder(int order);
    virtual int getDataOrder();

    virtual void activateChannel(int channel);
    virtual void deactivateChannel(int channel);
    virtual void activateAllChannels();
    virtual void deactivateAllChannels();

    virtual int &channelData(int channel);
    virtual int &channelGain(int channel);    
    virtual string &channelName(int channel);

    virtual bool channelIsActive(int channel);
    virtual int numberOfActiveChannels();

    virtual void clearData();

    virtual int &flag(int flagNumber);

    virtual DataSet operator*(float modifier);

    DataType getDataType();

private:
    vector<int> dataSet;
    vector<bool> activeChannels;
    vector<int> flags;
    vector<int> channelGains;
    vector<string> channelNames;
    int dataOrder;
};

#endif // DATASET_H
