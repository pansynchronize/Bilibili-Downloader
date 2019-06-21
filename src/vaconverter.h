#ifndef VACONVERTER_H
#define VACONVERTER_H

#include <QDialog>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QTimer>
#include <QRegExp>

namespace Ui {
class VAConverter;
}

class VAConverter : public QDialog
{
    Q_OBJECT

public:
    explicit VAConverter(QWidget *parent = nullptr);
    ~VAConverter();

private slots:
    void on_pushButton_clicked();
    void on_lineEdit_clicked();
    void on_lineEdit_2_clicked();

    void on_pushButton_2_clicked();
signals:
    void progressUpdated(qint64 received, qint64 total);
private:
    Ui::VAConverter *ui;
};

#endif // VACONVERTER_H
