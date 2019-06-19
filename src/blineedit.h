#ifndef BLINEEDIT_H
#define BLINEEDIT_H
#include <QLineEdit>

class BLineEdit: public QLineEdit
{
    Q_OBJECT
public:
    BLineEdit(QWidget* parent=nullptr);
    void mousePressEvent(QMouseEvent *e) override;
signals:
    void clicked();
};

#endif // BLINEEDIT_H
