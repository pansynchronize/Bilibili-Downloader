#include "sourcechooser.h"

SourceChooser::SourceChooser(QWidget *parent): QDialog(parent)
{
    setWindowTitle(tr("Choose Video Resolution"));
    setWindowFlag(Qt::WindowType::Popup);
    setMinimumSize(QSize(360, 240));
    vbox = new QVBoxLayout(this);
    hbox = new QHBoxLayout;
    fbox = new QFormLayout;
    vbox->addLayout(fbox);
    vbox->addLayout(hbox);
    vbox->setStretchFactor(fbox, 10);
    vbox->setStretchFactor(hbox, 1);
    spacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding);
    hbox->addSpacerItem(spacer);
    cancel = new QPushButton(tr("Cancel"));
    confirm = new QPushButton(tr("Confirm"));
    hbox->addWidget(cancel);
    hbox->addWidget(confirm);
    p360 = new QRadioButton("360P");
    p480 = new QRadioButton("480P");
    p720 = new QRadioButton("720P");
    p1080 = new QRadioButton("1080P");
    p360->setEnabled(false);
    p480->setEnabled(false);
    p720->setEnabled(false);
    p1080->setEnabled(false);
    fbox->addRow(p360);
    fbox->addRow(p480);
    fbox->addRow(p720);
    fbox->addRow(p1080);
    connect(cancel, &QPushButton::clicked, this, [this](){
//        p360->setEnabled(false);
//        p480->setEnabled(false);
//        p720->setEnabled(false);
//        p1080->setEnabled(false);
        this->close();
    });
    connect(confirm, &QPushButton::clicked, this, [this](){
        if(p360->isChecked()){
            emit confirmed("360P");
            this->close();
        }else if(p480->isChecked()){
            emit confirmed("480P");
            this->close();
        }else if(p720->isChecked()){
            emit confirmed("720P");
            this->close();
        }else if(p1080->isChecked()){
            emit confirmed("1080P");
            this->close();
        }
//        p360->setEnabled(false);
//        p480->setEnabled(false);
//        p720->setEnabled(false);
//        p1080->setEnabled(false);

    });
}

void SourceChooser::addSource(const QStringList &lst)
{
    qDebug() << "Add source!" << endl;
    for(auto& i : lst){
        if(i == "360P"){
            p360->setEnabled(true);
        }else if(i == "480P"){
            p480->setEnabled(true);
        }else if(i == "720P"){
            p720->setEnabled(true);
        }else if(i == "1080P"){
            p1080->setEnabled(true);
        }
    }
}
