#ifndef SLICE_H
#define SLICE_H

#include <QtCharts/QPieSlice>

QT_CHARTS_USE_NAMESPACE

class Slice : public QPieSlice
{
    Q_OBJECT

public:
    Slice(QString p_label, qint8 p_value);

    void reset();
    void select();

    qint8 m_test;

public Q_SLOTS:
    void hoverCallback(bool p_state);

};

#endif // SLICE_H
