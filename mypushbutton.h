#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT

public:
    MyPushButton(const QString &p_text, const qint8 p_value);

    qint8 getValue() const { return m_value; }

private:

    qint8 m_value;
};

#endif // MYPUSHBUTTON_H
