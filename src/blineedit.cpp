#include "blineedit.h"

BLineEdit::BLineEdit(QWidget *parent): QLineEdit(parent)
{

}

void BLineEdit::mousePressEvent(QMouseEvent *e)
{
    emit clicked();
}
