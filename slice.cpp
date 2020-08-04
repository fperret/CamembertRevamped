#include "slice.h"

Slice::Slice(QString p_label, qint8 p_value) : QPieSlice(p_label, p_value), m_test(10)
{
    connect(this, &Slice::hovered, this, &Slice::hoverCallback);
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

void Slice::reset()
{
    setExploded(false);
}

void Slice::select()
{
    setExploded(true);
}
