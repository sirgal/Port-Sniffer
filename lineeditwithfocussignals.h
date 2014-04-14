#ifndef LINEEDITWITHFOCUSSIGNALS_H
#define LINEEDITWITHFOCUSSIGNALS_H

#include <QLineEdit>

class LineEditWithFocusSignals : public QLineEdit
{
    Q_OBJECT

public:
    explicit LineEditWithFocusSignals(QWidget *parent = 0) :
        QLineEdit(parent)
    { }

signals:
    void lostFocus();
    void gotFocus();

protected:
    virtual void focusInEvent(QFocusEvent *e)
    {
        QLineEdit::focusInEvent(e);
        emit gotFocus();
    }

    virtual void focusOutEvent(QFocusEvent *e)
    {
        QLineEdit::focusOutEvent(e);
        emit lostFocus();
    }
};

#endif // LINEEDITWITHFOCUSSIGNALS_H
