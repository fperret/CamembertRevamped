#ifndef MYMAINWIDGET_H
#define MYMAINWIDGET_H


#include "slicemodel.h"
#include "gear.h"
#include "monitoredqvector.h"

#include <QWidget>
#include <QtWidgets/QFormLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QComboBox>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPushButton>

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
    bool isNameInSeries(const QString &p_nameToFind);

    static bool saveValues(const QPieSeries *p_series);

public Q_SLOTS:
    void callbackSliceClicked(QPieSlice *p_slice);
    void callbackSliceDoubleClicked(QPieSlice *p_slice);
    void addArmorSlice();
    void addWeaponSlice();
    void deleteSlice();
    void loadNewChart(const QString &p_title);
    void deleteChartAction();

private:

    void createChartForKey(const QString &p_key);

    void createChartSelectionList(QGridLayout *p_parentLayout);

    void createItemsForGearAddLists();

    QChart      *m_chart;
    QChartView  *m_chartView;
    Slice       *m_selectedSlice;
    QPieSeries  *m_series;
    QComboBox   *m_newWeaponSelection;
    QComboBox   *m_newArmorSelection;
    QComboBox   *m_chartSelection;
    QPushButton *m_deleteChartButton;


    QGridLayout *m_infoGridLayout;
    QVBoxLayout *m_sliceInfoLayout;

    QVector<SliceModel *> m_sliceModels;


signals:

};

#endif // MYMAINWIDGET_H
