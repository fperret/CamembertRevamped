#include "mymainwidget.h"
#include "mainwindow.h"

QT_CHARTS_USE_NAMESPACE

MyMainWidget::MyMainWidget(QWidget *parent) : QWidget(parent), m_chartView(0), m_selectedSlice(0)
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

    QPieSeries *lp_pieSeries = new QPieSeries();
    *lp_pieSeries << new Slice("Slice 1", 10);
    *lp_pieSeries << new Slice("Slice 2", 20);
    *lp_pieSeries << new Slice("Slice 3", 0);
    lp_pieSeries->setLabelsVisible(true);

    lp_chart->addSeries(lp_pieSeries);

    connect(lp_pieSeries, &QPieSeries::clicked, this, &MyMainWidget::callbackSliceClicked);
    connect(lp_pieSeries, &QPieSeries::doubleClicked, this, &MyMainWidget::callbackSliceDoubleClicked);

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
}
