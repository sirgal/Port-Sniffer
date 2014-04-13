#include "lineeditwithfocussignals.h"

LineEditWithFocusSignals::LineEditWithFocusSignals(QWidget *parent) :
    QLineEdit(parent)
{
}

void LineEditWithFocusSignals::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    emit gotFocus();
}

void LineEditWithFocusSignals::focusOutEvent(QFocusEvent *e)
{
    QLineEdit::focusOutEvent(e);
    emit lostFocus();
}
