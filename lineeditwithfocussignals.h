#ifndef LINEEDITWITHFOCUSSIGNALS_H
#define LINEEDITWITHFOCUSSIGNALS_H

#include <QLineEdit>

class LineEditWithFocusSignals : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEditWithFocusSignals(QWidget *parent = 0);

signals:
    void lostFocus();
    void gotFocus();

public slots:

protected:
    virtual void focusInEvent(QFocusEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);
};

#endif // LINEEDITWITHFOCUSSIGNALS_H
