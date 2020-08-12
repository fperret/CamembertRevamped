#ifndef MONITOREDQVECTOR_H
#define MONITOREDQVECTOR_H

#include <QObject>
#include <QVector>
#include "gear.h"

// See if we can templatize this
class MonitoredQVector : public QObject
{
    Q_OBJECT

public:
    MonitoredQVector();

    void push_back(const Gear &p_gear);

    QVector<Gear>::const_iterator cbegin();
    QVector<Gear>::const_iterator cend();
    const Gear &back();

signals:
    void contentChanged();


private:
    QVector<Gear> m_vector;
};

#endif // MONITOREDQVECTOR_H
