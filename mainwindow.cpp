#include "mainwindow.h"
#include "ui_mainwindow.h"

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QChart *lp_chart = new QChart();
    lp_chart->setTitle("Test title");

    QPieSeries *lp_pieSeries = new QPieSeries();
    *lp_pieSeries << new QPieSlice("Slice 1", 10, this);
    lp_pieSeries->setLabelsVisible(true);

    lp_chart->addSeries(lp_pieSeries);

    ui->myChartView->setChart(lp_chart);

}

MainWindow::~MainWindow()
{
    delete ui;
}

