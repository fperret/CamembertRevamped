#ifndef SLICEMODEL_H
#define SLICEMODEL_H

#include "slice.h"
#include <QLayout>
#include <QLabel>

QT_CHARTS_USE_NAMESPACE

class SliceModel : public QObject
{
    Q_OBJECT

public:
    SliceModel(QVBoxLayout *p_parentLayout, const QString &p_sliceLabel, const qint8 &p_value, const QString &p_sliceName, QPieSeries *p_owner);
    ~SliceModel();

    Slice *getSlice() const { return m_slice; }
    QHBoxLayout *getRowContainer() const { return m_rowContainer; }

public Q_SLOTS:
    void callbackValueChanged();

private:

    void createSectionRow();

    Slice           *m_slice;
    QHBoxLayout     *m_rowContainer;
    QLabel          *m_infoLabel;
    QVBoxLayout     *m_parentLayout;

    qint8 m_value;
};

#endif // SLICEMODEL_H
