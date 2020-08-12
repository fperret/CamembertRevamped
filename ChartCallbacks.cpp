#include "mymainwidget.h"
#include "mainwindow.h"

QT_CHARTS_USE_NAMESPACE

void MyMainWidget::callbackSliceClicked(QPieSlice *p_slice)
{
    // TODO : manage clicking on the same slice
    if (m_selectedSlice != 0)
    {
        if (m_selectedSlice == p_slice)
            return ;
        else
        {
            m_selectedSlice->setBorderColor(Qt::white);
            m_selectedSlice->setBorderWidth(1);
        }
    }

    if (p_slice != 0) {
        m_selectedSlice = dynamic_cast<Slice *>(p_slice);

        m_selectedSlice->setBorderColor(Qt::red);
        m_selectedSlice->setBorderWidth(3);
    }
}


void MyMainWidget::callbackSliceDoubleClicked(QPieSlice *p_slice)
{
    p_slice->setValue(p_slice->value() + 1);
}
