#ifndef BIPROGRESSBAR_H
#define BIPROGRESSBAR_H
#include <QProgressBar>
#include <QMouseEvent>

class BiProgressBar: public QProgressBar
{
    Q_OBJECT
    bool pressed;
public:
    BiProgressBar(QWidget* parent=nullptr);
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;

signals:
    void clicked(QMouseEvent* e);
};

#endif // BIPROGRESSBAR_H
