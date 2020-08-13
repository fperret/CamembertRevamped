#include "slicemodel.h"
#include "mymainwidget.h"

#include "mypushbutton.h"

QT_CHARTS_USE_NAMESPACE

SliceModel::SliceModel(QVBoxLayout *p_parentLayout, const QString &p_sliceLabel, const qint8 &p_value, const QString &p_sliceName, QPieSeries *p_owner) :
    m_parentLayout(p_parentLayout),
    m_value(p_value)
{
    m_slice = new Slice(p_sliceLabel, m_value, p_sliceName);
    *p_owner << m_slice;

    createSectionRow();

    // Must be called at the end to be sure all pointers are set up
    connect(m_slice, &Slice::valueChanged, this, &SliceModel::callbackValueChanged);

    // Call this once manually to initialize everything
    callbackValueChanged();
}

SliceModel::~SliceModel()
{
    if (m_parentLayout != 0) {
        //m_parentLayout->removeItem()
    }
}

void SliceModel::createSectionRow()
{
    m_infoLabel = new QLabel(m_slice->label());
    m_infoLabel->setFrameStyle(QFrame::Box);

    MyPushButton *l_minus = new MyPushButton("-", -1);
    MyPushButton *l_plus = new MyPushButton("+", 1);
    connect(l_minus, &MyPushButton::clicked, m_slice, &Slice::callbackButtonClick);
    connect(l_plus, &MyPushButton::clicked, m_slice, &Slice::callbackButtonClick);

    m_rowContainer = new QHBoxLayout();
    m_rowContainer->addWidget(l_minus);
    m_rowContainer->addWidget(m_infoLabel);
    m_rowContainer->addWidget(l_plus);

    if (m_parentLayout != 0) {
        m_parentLayout->addLayout(m_rowContainer);
    }
}

void SliceModel::callbackValueChanged()
{
    qDebug() << "old value : " << m_value << " / new value : " << m_slice->value();

    m_value = m_slice->value();

    m_slice->setLabel(m_slice->objectName() + " : " + QString::number(m_value));
    m_infoLabel->setText(m_slice->label());

    MyMainWidget::saveValues(m_slice->series());
}
