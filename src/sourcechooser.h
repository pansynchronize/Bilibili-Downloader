#ifndef SOURCECHOOSER_H
#define SOURCECHOOSER_H

#include <QDialog>
#include <QBoxLayout>
#include <QFormLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QRadioButton>
#include <QDebug>

class SourceChooser: public QDialog
{
    Q_OBJECT
    QVBoxLayout* vbox;
    QHBoxLayout* hbox;
    QFormLayout* fbox;
    QSpacerItem* spacer;
    QPushButton* cancel;
    QPushButton* confirm;
    QRadioButton* p360;
    QRadioButton* p480;
    QRadioButton* p720;
    QRadioButton* p1080;
public:
    SourceChooser(QWidget* parent = nullptr);
    void addSource(const QStringList& lst);
signals:
    void confirmed(QString resolution);
};

#endif // SOURCECHOOSER_H
