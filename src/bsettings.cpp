#include "bsettings.h"
#include "ui_bsettings.h"

BSettings::BSettings(QSettings& setting, QWidget *parent):
    QDialog(parent),
ui(new Ui::BSettings)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::WindowType::Popup);
    QLineEdit* child;
    foreach(child, this->findChildren<QLineEdit*>()){
        child->setText(setting.value(child->objectName()).toString());
    }
}

BSettings::~BSettings()
{
    delete ui;
}

void BSettings::on_path_clicked()
{
    qDebug() << "Clicked! " << endl;
    QString path = QFileDialog::getExistingDirectory(this, tr("Download to"), QDir::homePath());
    if(!path.isEmpty()){
        ui->path->setText(path);
    }
}

void BSettings::on_pushButton_clicked()
{
    this->close();
}

void BSettings::on_pushButton_2_clicked()
{
    QLineEdit* lineedit;
    foreach (lineedit, this->findChildren<QLineEdit*>()) {
        settings.insert(lineedit->objectName(), lineedit->text());
    }
    emit applied(settings);
    this->close();
}
