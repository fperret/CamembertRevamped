#include "slice.h"
#include "mymainwidget.h"
#include "mypushbutton.h"

QT_CHARTS_USE_NAMESPACE

Slice::Slice(const QString &p_label, const qint8 &p_value, const QString &p_name) : QPieSlice(p_label, p_value)
{
    connect(this, &Slice::hovered, this, &Slice::hoverCallback);
    setObjectName(p_name);
    setLabelVisible(true);
    setLabelPosition(LabelPosition::LabelOutside);

    QFont l_labelFont = labelFont();
    l_labelFont.setWeight(11);
    l_labelFont.setBold(true);
    setLabelFont(l_labelFont);
}

void Slice::hoverCallback(bool p_isHovered)
{
    if (p_isHovered) {
        // Save the color to restore it
        QBrush brush = this->brush();
        m_originalBrush = brush;
        brush.setColor(brush.color().lighter());
        setBrush(brush);
    } else {
        setBrush(m_originalBrush);
    }
}

void Slice::callbackButtonClick()
{
    MyPushButton *l_senderButton = qobject_cast<MyPushButton *>(sender());
    if (value() + l_senderButton->getValue() > 0) {
        setValue(value() + l_senderButton->getValue());
    }
}
