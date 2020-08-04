#include "mymainwidget.h"
#include "mainwindow.h"

QT_CHARTS_USE_NAMESPACE

MyMainWidget::MyMainWidget(QWidget *parent) : QWidget(parent), m_chartView(0), m_selectedSlice(0), m_series(0)
{
    createChart();

    QGridLayout *lp_baseLayout = new QGridLayout();
    lp_baseLayout->addWidget(m_chartView, 0, 0);
    setLayout(lp_baseLayout);
}

void MyMainWidget::createChart()
{
    QChart *lp_chart = new QChart();
    lp_chart->setTitle("Test title");

    // Create the data holder for the chart
    m_series = new QPieSeries();
    *m_series << new Slice("Slice 1", 10, "toto");
    *m_series << new Slice("Slice 2", 20, "tata");
    m_series->setLabelsVisible(true);
    m_series->setLabelsPosition(QPieSlice::LabelInsideNormal);


    // Link the data to the chart
    lp_chart->addSeries(m_series);

    // Set events related to the Slices
    connect(m_series, &QPieSeries::clicked, this, &MyMainWidget::callbackSliceClicked);
    connect(m_series, &QPieSeries::doubleClicked, this, &MyMainWidget::callbackSliceDoubleClicked);

    // Chart view
    m_chartView = new QChartView(lp_chart);
}


void MyMainWidget::keyReleaseEvent(QKeyEvent *p_event)
{
    // When we press 'u' while a slice is selected we decrement the value by one down to 0
    if (p_event->key() == Qt::Key_U)
    {
        if (m_selectedSlice != 0 && m_selectedSlice->value() > 0)
        {
            m_selectedSlice->setValue(m_selectedSlice->value() - 1);
            //m_selectedSlice->update();
        }
    }
}


void MyMainWidget::callbackSliceClicked(QPieSlice *p_slice)
{
    // TODO : manage clicking on the same slice
    if (m_selectedSlice)
    {
        if (m_selectedSlice == p_slice)
            return ;
        m_selectedSlice->reset();
    }

    m_selectedSlice = dynamic_cast<Slice *>(p_slice);
    m_selectedSlice->select();
}


void MyMainWidget::callbackSliceDoubleClicked(QPieSlice *p_slice)
{
    p_slice->setValue(p_slice->value() + 1);
    //dynamic_cast<Slice *>(p_slice)->update();
}

bool MyMainWidget::saveValues(const QPieSeries *p_series)
{
    if (p_series == 0)
        return false;

    QJsonObject l_jsonObject;
    for (auto slice : p_series->slices())
    {
        l_jsonObject[slice->objectName()] = slice->value();
    }
    logJson(l_jsonObject);
    return true;
}

void MyMainWidget::logJson(const QJsonObject &p_jsonObject)
{
    for (QJsonObject::const_iterator l_it = p_jsonObject.begin(); l_it != p_jsonObject.end(); ++l_it)
    {
        qDebug() << l_it.key() << " / " << l_it.value();
    }
    qDebug() << "---------------------------------";
}
