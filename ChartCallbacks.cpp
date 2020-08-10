#include "mymainwidget.h"
#include "mainwindow.h"

QT_CHARTS_USE_NAMESPACE

void MyMainWidget::callbackSliceClicked(QPieSlice *p_slice)
{
    // TODO : manage clicking on the same slice
    if (m_selectedSlice)
    {
        if (m_selectedSlice == p_slice)
            return ;
    }

    m_selectedSlice = dynamic_cast<Slice *>(p_slice);
}


void MyMainWidget::callbackSliceDoubleClicked(QPieSlice *p_slice)
{
    p_slice->setValue(p_slice->value() + 1);
}
