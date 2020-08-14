#include "mymainwidget.h"
#include "mainwindow.h"
#include "Utility.h"
#include "mypushbutton.h"

#include "slicemodel.h"

#include <QLabel>
#include <QPushButton>
#include <tuple>

QT_CHARTS_USE_NAMESPACE

static const std::string k_saveFileName = "../CamembertSave.json";

MyMainWidget::MyMainWidget(QWidget *parent) : QWidget(parent), m_chartView(0), m_selectedSlice(0), m_series(0)
{
    m_infoGridLayout = new QGridLayout();
    m_sliceInfoLayout = new QVBoxLayout();

    m_infoGridLayout->addLayout(m_sliceInfoLayout, 0, 0);

    m_chart = new QChart();

    m_series = new QPieSeries();
    // Data holder for the chart
    m_series->setLabelsVisible(true);
    m_series->setLabelsPosition(QPieSlice::LabelInsideNormal);
    m_chart->addSeries(m_series);

    m_chartView = new QChartView(m_chart);

    //createChart();
    createChartForKey("");
    createChartForKey("");
    //qDebug() << "after";

    QGridLayout *lp_baseLayout = new QGridLayout(this);
    lp_baseLayout->addWidget(m_chartView, 0, 1);
    setLayout(lp_baseLayout);

    QPushButton *l_addArmorButton = new QPushButton("Add armor", this);
    QPushButton *l_addWeaponButton = new QPushButton("Add weapon", this);
    // Leak need parent ?
    QVBoxLayout *l_armorAddBox = new QVBoxLayout();
    QVBoxLayout *l_weaponAddBox = new QVBoxLayout();

    // /!\ The name in the drop down list are not made the same way as the one in the slices
    m_newArmorSelection = new QComboBox(this);
    m_newWeaponSelection = new QComboBox(this);

    for (int l_rarityIt = 0; l_rarityIt != Gear::LAST_RARITY; l_rarityIt++) {

        Gear::RARITY l_rarity = static_cast<Gear::RARITY>(l_rarityIt);
        QString l_rarityStr = Gear::rarityToString(static_cast<Gear::RARITY>(l_rarityIt));

        for (int l_armorIt = 0; l_armorIt != Gear::LAST_ARMOR; l_armorIt++) {
            // Used only to get the name in the drop down list
            Gear l_tmpArmor(static_cast<Gear::ARMOR_TYPE>(l_armorIt), Gear::NOT_WEAPON, l_rarity);

            if (!isNameInSeries(l_tmpArmor.getName())) {
                m_newArmorSelection->addItem(l_tmpArmor.getName());
            }
        }

        for (int l_weaponIt = 0; l_weaponIt != Gear::LAST_WEAPON; l_weaponIt++) {
            // Used only to get the name in the drop down list
            Gear l_tmpWeapon(Gear::NOT_ARMOR, static_cast<Gear::WEAPON_TYPE>(l_weaponIt), l_rarity);

            if (!isNameInSeries(l_tmpWeapon.getName())) {
                m_newWeaponSelection->addItem(l_tmpWeapon.getName());
            }
        }
    }

    l_armorAddBox->addWidget(m_newArmorSelection);
    l_armorAddBox->addWidget(l_addArmorButton);

    l_weaponAddBox->addWidget(m_newWeaponSelection);
    l_weaponAddBox->addWidget(l_addWeaponButton);

    QPushButton *l_removeSlice = new QPushButton("Remove selected slice");

    QVBoxLayout *l_seriesInfoLayout = new QVBoxLayout();
    l_seriesInfoLayout->addLayout(l_armorAddBox);
    l_seriesInfoLayout->addLayout(l_weaponAddBox);
    l_seriesInfoLayout->addWidget(l_removeSlice);

    m_infoGridLayout->addLayout(l_seriesInfoLayout, 1, 0);

    connect(l_addArmorButton, &QPushButton::clicked, this, &MyMainWidget::addArmorSlice);
    connect(l_addWeaponButton, &QPushButton::clicked, this, &MyMainWidget::addWeaponSlice);
    connect(l_removeSlice, &QPushButton::clicked, this, &MyMainWidget::deleteSlice);

    lp_baseLayout->addLayout(m_infoGridLayout, 0, 0);
}

bool MyMainWidget::isNameInSeries(const QString &p_nameToFind)
{
    return (m_series->findChild<Slice *>(p_nameToFind) != nullptr);
}

MyMainWidget::~MyMainWidget()
{
    for (auto l_it : m_sliceModels)
    {
        delete l_it;
    }
}

void MyMainWidget::addArmorSlice()
{
    m_sliceModels.push_back(new SliceModel(m_sliceInfoLayout, "", 1, m_newArmorSelection->currentText(), m_series));
    m_newArmorSelection->removeItem(m_newArmorSelection->currentIndex());
}

void MyMainWidget::addWeaponSlice()
{
    m_sliceModels.push_back(new SliceModel(m_sliceInfoLayout, "", 1, m_newWeaponSelection->currentText(), m_series));
    m_newWeaponSelection->removeItem(m_newWeaponSelection->currentIndex());
}

void MyMainWidget::createChartForKey(const QString &p_key)
{
    m_series->clear();

    m_chart->setTitle(p_key);

    QJsonObject l_jsonObject;
    if (loadJsonObjectFromFile(l_jsonObject, k_saveFileName)) {
        for (auto l_key : l_jsonObject.keys()) {
            if (l_jsonObject.value(l_key).isDouble()) {
                //qDebug() << l_key << " : " << l_jsonObject.value(l_key);
                m_sliceModels.push_back(new SliceModel(m_sliceInfoLayout, "toto", l_jsonObject.value(l_key).toDouble(), l_key, m_series));
            }
        }
    }

    // Set events related to the Slices
    connect(m_series, &QPieSeries::clicked, this, &MyMainWidget::callbackSliceClicked);
    connect(m_series, &QPieSeries::doubleClicked, this, &MyMainWidget::callbackSliceDoubleClicked);
    // TODO : find why m_series become empty when we close the application
    connect(m_series, &QPieSeries::countChanged, this, [=]() { saveValues(m_series); });
}

bool MyMainWidget::saveValues(const QPieSeries *p_series)
{
    qDebug() << "Save on key : " << p_series->chart()->title();

    // For some reason when we close the window, m_series become empty and QPieSeries::countChanged is emitted with so this callback is triggered
    // If we do not check against empty series we lose all data
    if (p_series == 0 || p_series->isEmpty())
        return false;

    // Serialize the data in a json object
    QJsonObject l_jsonObject;
    for (auto slice : p_series->slices())
    {
        l_jsonObject[slice->objectName()] = slice->value();
    }

    //logJson(l_jsonObject);

    // Save the json in a file
    return saveJsonObject(l_jsonObject, k_saveFileName);
}

void MyMainWidget::deleteSlice()
{
    if (m_selectedSlice != 0) {
        m_series->remove(m_selectedSlice);
        for (auto it = m_sliceModels.begin(); it != m_sliceModels.end(); ++it) {
            if ((*it)->getSlice() == m_selectedSlice) {
                m_selectedSlice = 0;
                delete *it;
                m_sliceModels.erase(it);
                break;
            }
        }
    }
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
