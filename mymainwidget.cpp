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
    createChart();

    QGridLayout *lp_baseLayout = new QGridLayout(this);
    lp_baseLayout->addWidget(m_chartView, 0, 1);
    setLayout(lp_baseLayout);


    QGridLayout *l_parentGridLayout = new QGridLayout();
    createInfoArea(l_parentGridLayout);

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

            m_newArmorSelection->addItem(l_tmpArmor.getName());
        }

        for (int l_weaponIt = 0; l_weaponIt != Gear::LAST_WEAPON; l_weaponIt++) {
            // Used only to get the name in the drop down list
            Gear l_tmpWeapon(Gear::NOT_ARMOR, static_cast<Gear::WEAPON_TYPE>(l_weaponIt), l_rarity);
            m_newWeaponSelection->addItem(l_tmpWeapon.getName());
        }
    }

    l_armorAddBox->addWidget(m_newArmorSelection);
    l_armorAddBox->addWidget(l_addArmorButton);

    l_weaponAddBox->addWidget(m_newWeaponSelection);
    l_weaponAddBox->addWidget(l_addWeaponButton);

    l_parentGridLayout->addLayout(l_armorAddBox, l_parentGridLayout->count() + 1, 0);
    l_parentGridLayout->addLayout(l_weaponAddBox, l_parentGridLayout->count() + 1, 0);

    connect(l_addArmorButton, &QPushButton::clicked, this, &MyMainWidget::addArmorSlice);
    connect(l_addWeaponButton, &QPushButton::clicked, this, &MyMainWidget::addWeaponSlice);

    lp_baseLayout->addLayout(l_parentGridLayout, 0, 0);
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
    // Will not work if the order in which elements are added to the drop list is modified
    int l_chosenIndex = m_newArmorSelection->currentIndex();
    Gear::ARMOR_TYPE l_armorType = static_cast<Gear::ARMOR_TYPE>(l_chosenIndex % Gear::LAST_ARMOR);
    Gear::RARITY l_rarity = static_cast<Gear::RARITY>(l_chosenIndex / Gear::LAST_ARMOR);
    Gear l_newArmor(l_armorType, Gear::NOT_WEAPON, l_rarity);

    m_sliceModels.push_back(new SliceModel("", 1, l_newArmor.getName(), m_series));

    // For now we keep a track of that in case we want to use it in the future
    //m_armors.push_back(l_newArmor);
}

void MyMainWidget::addWeaponSlice()
{
    // Will not work if the order in which elements are added to the drop list is modified
    int l_chosenIndex = m_newArmorSelection->currentIndex();
    Gear::WEAPON_TYPE l_weaponType = static_cast<Gear::WEAPON_TYPE>(l_chosenIndex % Gear::LAST_WEAPON);
    Gear::RARITY l_rarity = static_cast<Gear::RARITY>(l_chosenIndex / Gear::LAST_WEAPON);
    Gear l_newWeapon(Gear::NOT_ARMOR, l_weaponType, l_rarity);

    m_sliceModels.push_back(new SliceModel("", 1, l_newWeapon.getName(), m_series));

    // For now we keep a track of that in case we want to use it in the future
    //m_weapons.push_back(l_newWeapon);
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
    // Parent ?
    QChart *lp_chart = new QChart();
    lp_chart->setTitle("Test title");

    // Create the data holder for the chart
    m_series = new QPieSeries(this);
    m_series->setLabelsVisible(true);
    m_series->setLabelsPosition(QPieSlice::LabelInsideNormal);

    QJsonObject l_jsonObject;
    if (loadJsonObjectFromFile(l_jsonObject, k_saveFileName)) {
        for (auto l_key : l_jsonObject.keys()) {
            if (l_jsonObject.value(l_key).isDouble()) {
                qDebug() << l_key << " : " << l_jsonObject.value(l_key);
                m_sliceModels.push_back(new SliceModel("", l_jsonObject.value(l_key).toDouble(), l_key, m_series));
            }
        }
    }

    // Link the data to the chart
    lp_chart->addSeries(m_series);

    // Set events related to the Slices
    connect(m_series, &QPieSeries::clicked, this, &MyMainWidget::callbackSliceClicked);
    connect(m_series, &QPieSeries::doubleClicked, this, &MyMainWidget::callbackSliceDoubleClicked);
    // TODO : find why m_series become empty when we close the application
    connect(m_series, &QPieSeries::countChanged, this, [=]() { saveValues(m_series); });
    // Chart view
    m_chartView = new QChartView(lp_chart);
}

bool MyMainWidget::saveValues(const QPieSeries *p_series)
{
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
