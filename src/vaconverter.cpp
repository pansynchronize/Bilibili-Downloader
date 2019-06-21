#include "vaconverter.h"
#include "ui_vaconverter.h"

VAConverter::VAConverter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VAConverter)
{
    ui->setupUi(this);
    ui->lineEdit_3->setText("Ready!");
    ui->pushButton->setDefault(false);
    ui->pushButton_2->setDefault(true);
}

VAConverter::~VAConverter()
{
    delete ui;
}

void VAConverter::on_pushButton_clicked()
{
    this->close();
}

void VAConverter::on_lineEdit_clicked()
{
    qDebug() << "Line Edit clicked! " << endl;
    QString input = QFileDialog::getOpenFileName(this, tr("Input Video File"), QDir::homePath(), tr("Video (*.mp4 *.mkv *.flv)"));
    if(!input.isEmpty()){
        ui->lineEdit->setText(input);
    }
}

void VAConverter::on_lineEdit_2_clicked()
{
    qDebug() << "Line Edit 2 clicked! " << endl;
    QString output = QFileDialog::getSaveFileName(this, tr("Output audio file"), QDir::homePath()+"/untitled.mp3", tr("Audio (*.mp3 *.wav)"));
    if(!output.isEmpty()){
        ui->lineEdit_2->setText(output);
    }
}

void VAConverter::on_pushButton_2_clicked()
{
    if(ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty()){
        QMessageBox::warning(this, tr("Warning"), tr("No input or no output!"));
    }else{
        //do something
        ui->pushButton_2->setEnabled(false);
        ui->lineEdit->setEnabled(false);
        ui->lineEdit_2->setEnabled(false);
        ui->lineEdit_3->setText("Processing...");
        QProcess* process = new QProcess(this);
        QString command = "ffmpeg -i \""+ui->lineEdit->text()+"\" -vn \""+ui->lineEdit_2->text()+"\" -y";
//        qDebug() << command << endl;
        connect(process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, [=](int exitcode){
//            qDebug() << process->readAllStandardError() << endl;
            if(exitcode == 0){
                //do something
            }else{
                qDebug() << "Process Crashed!" << endl;
            }
//            timer->deleteLater();
            process->deleteLater();
            ui->pushButton_2->setEnabled(true);
            ui->lineEdit->setEnabled(true);
            ui->lineEdit_2->setEnabled(true);
            ui->lineEdit_3->setText("Done!");
            QTimer* timer = new QTimer;
            timer->start(3000);
            connect(timer, &QTimer::timeout, this, [=]{
                 ui->lineEdit_3->setText("Ready!");
                 timer->stop();
                 timer->deleteLater();
            });
        });
//        timer->start(100);
        process->start(command);

    }
}
