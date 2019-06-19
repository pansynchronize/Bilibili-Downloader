#ifndef BSETTINGS_H
#define BSETTINGS_H

#include <QDialog>
#include <QDebug>
#include <QFileDialog>
#include <QObject>
#include <QSettings>

namespace Ui {
class BSettings;
}

class BSettings : public QDialog
{
    Q_OBJECT

public:
    explicit BSettings(QSettings& setting, QWidget *parent = nullptr);
    ~BSettings();
    QMap<QString, QString> settings;

private slots:
    void on_path_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::BSettings *ui;
signals:
    void applied(QMap<QString, QString> settings);
};

#endif // BSETTINGS_H
