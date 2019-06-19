#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <algorithm>
#include <QDebug>
#include <QCryptographicHash>
#include <QPair>
#include <QRegExp>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QMap>
#include "sourcechooser.h"
#include <QDataStream>
#include <QSettings>
#include <QFileDialog>
#include "bsettings.h"
#include <thread>
#include <QProcess>
#include <QSet>
#include <QDesktopServices>
//#include <QThread>
//#include <QMutex>

namespace Ui {
class MainWindow;
}

class UrlReceiver: public QObject{
private:
    Q_OBJECT
public:
    void process(QString start_url, QString cid, QString quality);
signals:
    void finished(QByteArray ba);
};

class Downloader: public QObject{
private:
    Q_OBJECT
    QNetworkAccessManager* dataManager;
//    QNetworkReply* reply;
    QMap<QString, QStringList> urllist;
    QMap<QString, QString> datalist;
    QJsonDocument jd;
    QString dstPath;
    QJsonArray ja;
    int counter = 0;
    QWidget* parent;
    QMap<uint8_t, qint64> totaldata, receiveddata;
public:
    explicit Downloader(QWidget* parent=nullptr);
    ~Downloader();
    void getPlayList(QString start_url, QString cid, QString quality);
    void getPlayLists(const QString& start_url, const QString& cid);
    void getMetaData(QString baseurl);
    void saveData(QString aid, QString cid, QString title, QString pic,
                  QString author, QString duration, QString likes, QString favourites,
                  QString coins, QString views);
    void saveData(const QByteArray& ba);
    QString start_url;
    QMap<QString, QString> getData() const;
    QMap<QString, QStringList> getUrls() const;
    void download(const QStringList& url, QString start_url, QString path);
//    void _download(const QStringList& url, QString start_url, QString path);
    long long sum(const QList<long long>& list);
public slots:
    void saveUrls(const QByteArray& ba);
    void urlChecker();
//    void writeFile(QNetworkReply* reply);
signals:
    void dataUpdated(QMap<QString, QString> datalist);
    void urlUpdated(QMap<QString, QStringList> urllist);
    void urlAdded();
    void operate(QString start_url, QString cid, QString quality);
    void threadReady();
    void downloaded(QString dstpath, QString basefilename);
    void dataReceived(qint64 received, qint64 total);
};


class MainWindow : public QMainWindow
{
    Q_OBJECT
    Downloader* downloader;
    QMap<QString, QStringList> urllist;
    SourceChooser* sc;
    BSettings* bs;
    QSettings settings;
    QString path;
    QPixmap* pixmap;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showData(const QMap<QString, QString>& datalist);
    void updateProgressBar(qint64 received, qint64 total);
    int numVideos = 0;
private slots:
    void on_actionClose_triggered();
    void on_actionAbout_triggered();
    void on_pushButton_clicked();

    void on_lineEdit_returnPressed();

    void on_actionPreferences_triggered();

private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
