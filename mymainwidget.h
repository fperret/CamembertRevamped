#ifndef MYMAINWIDGET_H
#define MYMAINWIDGET_H

#include "slice.h"

#include <QWidget>
#include <QtWidgets/QFormLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QDebug>
#include <QJsonObject>

QT_CHARTS_USE_NAMESPACE

class MyMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyMainWidget(QWidget *parent = nullptr);

    // Qt override
    void keyReleaseEvent(QKeyEvent *p_event);

    // Custom methods
    //bool saveValues();

    static void logJson(const QJsonObject &p_jsonObject);
    static bool saveValues(const QPieSeries *p_series);

public Q_SLOTS:
    void callbackSliceClicked(QPieSlice *p_slice);
    void callbackSliceDoubleClicked(QPieSlice *p_slice);

private:
    void createChart();


    QChartView *m_chartView;
    Slice       *m_selectedSlice;
    QPieSeries  *m_series;



signals:

};

#endif // MYMAINWIDGET_H
