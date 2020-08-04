#include "slice.h"
#include "mymainwidget.h"

QT_CHARTS_USE_NAMESPACE

Slice::Slice(const QString &p_label, const qint8 &p_value, const QString &p_name) : QPieSlice(p_label, p_value)
{
    connect(this, &Slice::hovered, this, &Slice::hoverCallback);
    connect(this, &Slice::valueChanged, this, &Slice::update);
    setObjectName(p_name);
    update();
}

void Slice::hoverCallback(bool p_isHovered)
{
    if (p_isHovered)
    {
        setColor(color().lighter());
    }
    else
    {
        setColor(color().darker());
    }
}

void Slice::update()
{
    setLabel(QString::number(value()));

    MyMainWidget::saveValues(series());
}

void Slice::reset()
{
    setExploded(false);
}

void Slice::select()
{
    setExploded(true);
}
