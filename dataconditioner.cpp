#include "dataconditioner.h"

DataConditioner::DataConditioner(QObject *parent) : QObject(parent)
{
    byteNumber = 33;
}

DataConditioner::~DataConditioner()
{

}

void DataConditioner::newBytes(QByteArray bytes)
{
    for (int byteIndex = 0; byteIndex < bytes.size(); byteIndex++)
    {
        unsigned char byte = bytes[byteIndex];

        if ((byteNumber >= 33) && (byte == 0xA0))
        {
            byteNumber = 1;

            conditionedData.clearData();
        }

        else
        {
            ++byteNumber;

            if ((byteNumber > 2) && (byteNumber < 27))
            {
                int index = ((int) (byteNumber / 3)) - 1;

                switch (byteNumber % 3)
                {
                case 0:
                    conditionedData.channelData(index) = 0;
                    conditionedData.channelData(index) |= ((0xFF & byte) << 16);
                    break;

                case 1:
                    conditionedData.channelData(index) |= ((0xFF & byte) << 8);
                    break;

                case 2:
                    conditionedData.channelData(index) |= (0xFF & byte);
                    //conditionedData.channelData(index) &= 0xFFFFFF80;          // xxxxxxxx xxxxxxxx xxxxxxxx x0000000

//                    if ((conditionedData.channelData(index) & 0x00800000) > 0) // xxxxxxxx 1xxxxxxx xxxxxxxx xxxxxxxx
//                        conditionedData.channelData(index) |= 0xFF000000;      // 11111111 xxxxxxxx xxxxxxxx xxxxxxxx
//                    else
//                        conditionedData.channelData(index) &= 0x00FFFFFF;      // 00000000 xxxxxxxx xxxxxxxx xxxxxxxx
                    break;
                }
            }

            else if ((byteNumber >= 27) && (byteNumber < 33))
                conditionedData.flag(byteNumber - 27) = (int) byte;

            else
            {
                switch (byteNumber)
                {
                case 2:
                    conditionedData.setDataOrder((int) byte);
                    break;

                case 33:
                    emit dataReady(conditionedData);
                    break;
                }
            }
        }
    }

}

void DataConditioner::activateChannel(int channel)
{
    conditionedData.activateChannel(channel);
}

void DataConditioner::deactivateChannel(int channel)
{

    conditionedData.deactivateChannel(channel);
}

void DataConditioner::activateAllChannels()
{
    conditionedData.activateAllChannels();
}

void DataConditioner::deactivateAllChannels()
{
    conditionedData.deactivateAllChannels();
}
