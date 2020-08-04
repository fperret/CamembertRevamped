#ifndef SLICE_H
#define SLICE_H

#include <QtCharts/QPieSlice>

QT_CHARTS_USE_NAMESPACE

class MyMainWidget;

class Slice : public QPieSlice
{
    Q_OBJECT

public:
    Slice(const QString &p_label, const qint8 &p_value, const QString &p_name);

    void reset();
    void select();
    void update();

public Q_SLOTS:
    void hoverCallback(bool p_state);

};

#endif // SLICE_H
