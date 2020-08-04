#ifndef MYMAINWIDGET_H
#define MYMAINWIDGET_H

#include "slice.h"

#include <QWidget>
#include <QtWidgets/QFormLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QDebug>

QT_CHARTS_USE_NAMESPACE

class MyMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyMainWidget(QWidget *parent = nullptr);

    void keyReleaseEvent(QKeyEvent *p_event);

public Q_SLOTS:
    void callbackSliceClicked(QPieSlice *p_slice);
    void callbackSliceDoubleClicked(QPieSlice *p_slice);

private:
    void createChart();

    QChartView *m_chartView;
    Slice       *m_selectedSlice;


signals:

};

#endif // MYMAINWIDGET_H
