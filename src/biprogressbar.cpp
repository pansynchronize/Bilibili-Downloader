#include "biprogressbar.h"

BiProgressBar::BiProgressBar(QWidget *parent): QProgressBar(parent), pressed(false)
{

}

void BiProgressBar::mousePressEvent(QMouseEvent *event)
{
    pressed = true;
}

void BiProgressBar::mouseReleaseEvent(QMouseEvent *ev)
{
    if(pressed){
        emit clicked(ev);
        pressed = false;
    }
}

void BiProgressBar::mouseMoveEvent(QMouseEvent *ev)
{
    if(!this->rect().contains(ev->pos())){
        if(pressed){
            pressed = false;
        }
    }
}
