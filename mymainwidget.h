#ifndef MYMAINWIDGET_H
#define MYMAINWIDGET_H

//#include "slice.h"
#include "slicemodel.h"

#include <QWidget>
#include <QtWidgets/QFormLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QComboBox>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

#include <map>

QT_CHARTS_USE_NAMESPACE

class MyMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyMainWidget(QWidget *parent = nullptr);
    ~MyMainWidget();

    // Qt override
    void keyReleaseEvent(QKeyEvent *p_event);

    // Custom methods
    static bool saveValues(const QPieSeries *p_series);

public Q_SLOTS:
    void callbackSliceClicked(QPieSlice *p_slice);
    void callbackSliceDoubleClicked(QPieSlice *p_slice);
    void addArmorSlice();
    void addWeaponSlice();

private:
    void createChart();
    void createInfoArea(QGridLayout *p_parentGridLayout);

    //QHBoxLayout *createSectionRow(const std::string &p_label, Slice *p_connectedSlice);


    QChartView *m_chartView;
    Slice       *m_selectedSlice;
    QPieSeries  *m_series;
    QComboBox   *m_newWeaponSelection;
    QComboBox   *m_newArmorSelection;

    QVector<SliceModel *> m_sliceModels;


signals:

};

#endif // MYMAINWIDGET_H
