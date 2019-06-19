#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>

class PreviewWidget: public QLabel
{
    Q_OBJECT
    bool pressed;
public:
    PreviewWidget(QWidget* parent=nullptr);
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;

signals:
    void clicked(QMouseEvent* e);
};

#endif // PREVIEWWIDGET_H
