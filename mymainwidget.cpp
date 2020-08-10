#include "mymainwidget.h"
#include "mainwindow.h"
#include "Utility.h"
#include "mypushbutton.h"

#include "slicemodel.h"

#include <QLabel>
#include <QPushButton>

QT_CHARTS_USE_NAMESPACE

static const std::string k_saveFileName = "../CamembertSave.json";

MyMainWidget::MyMainWidget(QWidget *parent) : QWidget(parent), m_chartView(0), m_selectedSlice(0), m_series(0)
{
    createChart();

    QGridLayout *lp_baseLayout = new QGridLayout();
    lp_baseLayout->addWidget(m_chartView, 0, 1);
    setLayout(lp_baseLayout);

    QGridLayout *l_parentGridLayout = new QGridLayout();
    createInfoArea(l_parentGridLayout);
    lp_baseLayout->addLayout(l_parentGridLayout, 0, 0);

}

MyMainWidget::~MyMainWidget()
{
    for (auto l_it : m_sliceModels)
    {
        delete l_it;
    }
}

void MyMainWidget::createInfoArea(QGridLayout *p_parentGridLayout)
{
    int l_row = 0;
    for (auto sliceModel : m_sliceModels) {
        p_parentGridLayout->addLayout(sliceModel->getRowContainer(), l_row, 0);
        l_row++;
    }
}

void MyMainWidget::createChart()
{
    QChart *lp_chart = new QChart();
    lp_chart->setTitle("Test title");

    // Create the data holder for the chart
    m_series = new QPieSeries();
    m_series->setLabelsVisible(true);
    m_series->setLabelsPosition(QPieSlice::LabelInsideNormal);

    QJsonObject l_jsonObject;
    if (loadJsonObjectFromFile(l_jsonObject, k_saveFileName)) {
        for (auto l_key : l_jsonObject.keys()) {
            if (l_jsonObject.value(l_key).isDouble()) {
                m_sliceModels.push_back(new SliceModel("", l_jsonObject.value(l_key).toDouble(), l_key));

                *m_series << m_sliceModels.back()->getSlice();
            }
        }
    }

    // Link the data to the chart
    lp_chart->addSeries(m_series);

    // Set events related to the Slices
    connect(m_series, &QPieSeries::clicked, this, &MyMainWidget::callbackSliceClicked);
    connect(m_series, &QPieSeries::doubleClicked, this, &MyMainWidget::callbackSliceDoubleClicked);

    // Chart view
    m_chartView = new QChartView(lp_chart);
}

bool MyMainWidget::saveValues(const QPieSeries *p_series)
{
    if (p_series == 0)
        return false;

    // Serialize the data in a json object
    QJsonObject l_jsonObject;
    for (auto slice : p_series->slices())
    {
        l_jsonObject[slice->objectName()] = slice->value();
    }

    logJson(l_jsonObject);

    // Save the json in a file
    return saveJsonObject(l_jsonObject, k_saveFileName);
}



void MyMainWidget::keyReleaseEvent(QKeyEvent *p_event)
{
    // When we press 'u' while a slice is selected we decrement the value by one down to 0
    if (p_event->key() == Qt::Key_U) {
        if (m_selectedSlice != 0 && m_selectedSlice->value() > 0) {
            m_selectedSlice->setValue(m_selectedSlice->value() - 1);
        }
    }
}
