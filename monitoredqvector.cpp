#include "monitoredqvector.h"

MonitoredQVector::MonitoredQVector()
{
}

void MonitoredQVector::push_back(const Gear &p_gear)
{
    m_vector.push_back(p_gear);
    emit contentChanged();
}

QVector<Gear>::const_iterator MonitoredQVector::cbegin()
{
    return m_vector.cbegin();
}

QVector<Gear>::const_iterator MonitoredQVector::cend()
{
    return m_vector.cend();
}

const Gear &MonitoredQVector::back()
{
    return m_vector.back();
}
