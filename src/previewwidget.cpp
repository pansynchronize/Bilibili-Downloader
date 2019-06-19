#include "previewwidget.h"

PreviewWidget::PreviewWidget(QWidget *parent): QLabel(parent), pressed(false)
{

}

void PreviewWidget::mousePressEvent(QMouseEvent *ev)
{
    pressed = true;
}

void PreviewWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if(pressed){
        //do something
//        qDebug() << "Clicked!" << endl;
        emit clicked(ev);
        pressed = false;
    }
}

void PreviewWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if(!this->rect().contains(ev->pos())){
        if(pressed){
            pressed = false;
        }
    }
}
