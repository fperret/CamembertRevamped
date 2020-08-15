#include "mymainwidget.h"
#include "mainwindow.h"
#include "Utility.h"
#include "mypushbutton.h"

#include "slicemodel.h"

#include <QLabel>
#include <QPushButton>
#include <tuple>
#include <QLineEdit>
#include <QMessageBox>
#include <QApplication>

QT_CHARTS_USE_NAMESPACE

static const std::string k_saveFileName = "../CamembertSave.json";

MyMainWidget::MyMainWidget(QWidget *parent) : QWidget(parent),
    m_chart(0),
    m_chartView(0),
    m_selectedSlice(0),
    m_series(0),
    m_newWeaponSelection(0),
    m_newArmorSelection(0),
    m_infoGridLayout(0),
    m_sliceInfoLayout(0)
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

    QGridLayout *lp_baseLayout = new QGridLayout(this);
    createChartSelectionList(lp_baseLayout);


    lp_baseLayout->addWidget(m_chartView, 1, 1);
    setLayout(lp_baseLayout);

    QPushButton *l_addArmorButton = new QPushButton("Add armor", this);
    QPushButton *l_addWeaponButton = new QPushButton("Add weapon", this);
    // Leak need parent ?
    QVBoxLayout *l_armorAddBox = new QVBoxLayout();
    QVBoxLayout *l_weaponAddBox = new QVBoxLayout();

    m_newArmorSelection = new QComboBox(this);
    m_newWeaponSelection = new QComboBox(this);

    createItemsForGearAddLists();

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

    lp_baseLayout->addLayout(m_infoGridLayout, 1, 0);
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

void MyMainWidget::createChartSelectionList(QGridLayout *p_parentLayout)
{
    QHBoxLayout *l_chartSelectionArea = new QHBoxLayout();

    m_chartSelection = new QComboBox();
    QJsonObject l_jsonObject;
    if (loadJsonObjectFromFile(l_jsonObject, k_saveFileName)) {
        for (auto l_key : l_jsonObject.keys()) {
            if (l_jsonObject.value(l_key).isObject()) {
                m_chartSelection->addItem(l_key);
            }
        }
    }
    m_chartSelection->setCurrentIndex(-1);

    QLineEdit *l_newChartNameEdit = new QLineEdit();
    l_newChartNameEdit->setPlaceholderText("Enter new chart name");

    QPushButton *l_createChart = new QPushButton("Create new chart");
    connect(l_createChart, &QPushButton::clicked, this,
            [this, l_newChartNameEdit] () {
                if (m_chartSelection->findText(l_newChartNameEdit->text()) == -1) {
                    m_chartSelection->addItem(l_newChartNameEdit->text());
                    m_chartSelection->setCurrentText(l_newChartNameEdit->text());
                }
            }
    );

    m_deleteChartButton = new QPushButton("Delete current chart");
    connect(m_deleteChartButton, &QPushButton::clicked, this, &MyMainWidget::deleteChartAction);
    m_deleteChartButton->setDisabled(true);

    l_chartSelectionArea->addWidget(m_chartSelection);
    l_chartSelectionArea->addWidget(l_newChartNameEdit);
    l_chartSelectionArea->addWidget(l_createChart);
    l_chartSelectionArea->addWidget(m_deleteChartButton);

    p_parentLayout->addLayout(l_chartSelectionArea, 0, 0);

    connect(m_chartSelection, &QComboBox::currentTextChanged, this, &MyMainWidget::loadNewChart);
}

void MyMainWidget::deleteChartAction()
{
    QMessageBox::StandardButton l_confirmMessage;
    l_confirmMessage = QMessageBox::question(this, "Warning", "Delete chart for " + m_chart->title() + " ?", QMessageBox::No | QMessageBox::Yes);
    if (l_confirmMessage == QMessageBox::Yes) {
        // Delete JSON
        QJsonObject l_root;
        if (loadJsonObjectFromFile(l_root, k_saveFileName)) {
            auto l_keyEntry = l_root.find(m_chart->title());
            if (l_keyEntry != l_root.end()) {
                l_root.erase(l_keyEntry);
                saveJsonObject(l_root, k_saveFileName);
            }
        }

        // Remove from selection drop down
        m_chartSelection->removeItem(m_chartSelection->currentIndex());

        m_chartSelection->setCurrentIndex(0);
        loadNewChart(m_chartSelection->currentText());
    } else if (l_confirmMessage == QMessageBox::No) {
        qDebug() << "No pressed";
    }
}

void MyMainWidget::loadNewChart(const QString &p_title)
{
    // Cleanup from one display to another should be done here
    m_series->clear();
    for (auto l_it : m_sliceModels) {
        delete l_it;
    }
    m_sliceModels.clear();

    m_deleteChartButton->setDisabled(false);
    // Create everything for the new entry (should change name TODO)
    createChartForKey(p_title);
}

void MyMainWidget::createItemsForGearAddLists()
{
    Q_ASSERT(m_newArmorSelection != 0);
    Q_ASSERT(m_newWeaponSelection != 0);

    // We call this method when we initialize the drop down lists but also when we want to add items
    // that were previously deleted.
    // I don't know how to reinsert the items at the correct position in the second case
    // So it is easier to just clear the list and redo it
    m_newArmorSelection->clear();
    m_newWeaponSelection->clear();

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

    QJsonObject l_root;
    if (loadJsonObjectFromFile(l_root, k_saveFileName)) {
        auto l_keyEntry = l_root.find(p_key);
        if (l_keyEntry != l_root.end()) {
            for (auto l_leafKey : l_keyEntry.value().toObject().keys()) {
                QJsonValue l_leafValue = l_keyEntry.value().toObject().value(l_leafKey);
                  if (l_leafValue.isDouble()) {
                    m_sliceModels.push_back(new SliceModel(m_sliceInfoLayout, "toto", l_leafValue.toDouble(), l_leafKey, m_series));
                }
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
    // For some reason when we close the window, m_series become empty and QPieSeries::countChanged is emitted with so this callback is triggered
    // If we do not check against empty series we lose all data
    // We also clear the series when we move from one chart to another so it is not a bad idea to keep the isEmpty() check after all.
    if (p_series == 0 || p_series->isEmpty())
        return false;

    // Serialize the data in a json object
    QJsonObject l_root;
    if (loadJsonObjectFromFile(l_root, k_saveFileName)) {
        auto l_parent = l_root.find(p_series->chart()->title());

        QJsonObject l_new;
        for (auto slice : p_series->slices()){
            l_new[slice->objectName()] = slice->value();
        }

        if (l_parent == l_root.end()) {
            l_root.insert(p_series->chart()->title(), l_new);
        } else {
            l_parent.value() = l_new;
        }
    }

    logJson(l_root);

    // Save the json in a file
    return saveJsonObject(l_root, k_saveFileName);
}

void MyMainWidget::deleteSlice()
{
    if (m_selectedSlice != 0) {
        m_series->remove(m_selectedSlice);
        for (auto it = m_sliceModels.begin(); it != m_sliceModels.end(); ++it) {
            if ((*it)->getSlice() == m_selectedSlice) {
                // Reset selected slice
                m_selectedSlice = 0;

                // Delete the SliceModel object
                delete *it;
                m_sliceModels.erase(it);

                // Add the option for the delete slice in the relevant drop down list
                // To re add it
                createItemsForGearAddLists();

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
