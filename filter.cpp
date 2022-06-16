#include "filter.h"

Filter::Filter(QObject *parent) : QObject(parent)
{
    filterGain = 1;
}

Filter::~Filter()
{

}
