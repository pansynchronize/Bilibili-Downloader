#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), settings("XWare", "Bilibili Downloader"), pixmap(nullptr),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("./images/icon.svg"));
    QCoreApplication::setOrganizationName("XWare");
    QCoreApplication::setOrganizationDomain("XWare.com");
    QCoreApplication::setApplicationName("Bilibili Downloader");
    path = settings.value("path").toString();
    downloader = new Downloader(this);
    connect(ui->label_2, &PreviewWidget::clicked, this, [=](QMouseEvent* me){
        if(pixmap != nullptr){
            QString filename = QFileDialog::getSaveFileName(this, tr("Save to"), QDir::homePath()+"/Untitled.jpg", tr("Images (*.jpg *.png *.xpm)"));
            if(!filename.isEmpty()){
                QFile file(filename);
                file.open(QIODevice::WriteOnly);
                pixmap->save(&file);
                file.close();
            }
        }
    });
    connect(ui->progressBar, &BiProgressBar::clicked, this, [=](QMouseEvent* me){
        if(ui->progressBar->value() == 100){
            QString dir = QDir::toNativeSeparators(path);
            QDesktopServices::openUrl(dir);
        }
    });
    connect(downloader, &Downloader::dataUpdated, this, [this](const QMap<QString,QMap<QString, QString>>& datalist, QStringList pgs){
        ui->comboBox->setEnabled(true);
        ui->comboBox->clear();
        ui->comboBox->addItems(std::move(pgs));
        numofpages = ui->comboBox->count();
        alldata.clear();
        alldata = datalist;
//        qDebug() << datalist << endl;
        this->showData(alldata["page 1"], true);
    });
    connect(downloader, &Downloader::urlUpdated, this, [=](const QMap<QString, QStringList>& urllist){
       this->urllist = urllist;
       ui->pushButton->setEnabled(true);
       ui->comboBox->setEnabled(true);
    });
    connect(downloader, &Downloader::downloaded, this, [this](const QString& dstpath, const QString& basefilename){
        if(this->numVideos == 0){
            ui->statusBar->showMessage(tr("Merging and converting the downloaded video..."));
            QDir directory(dstpath);
            QStringList videos = directory.entryList(QStringList() << "*.flv", QDir::Files|QDir::Hidden);
            QProcess *p = new QProcess(this);
//            qDebug() << videos.length() << endl;
            if(videos.length() == 1){
                QString command = QString("ffmpeg -i \""+dstpath+"/"+videos[0]+"\" -codec copy \""+dstpath+"/"+basefilename+".mp4\" -y");
                qDebug() << command << endl;
                p->start(command);
                connect(p, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, [=](int exitcode){
                    qDebug() << QString().fromUtf8(p->readAllStandardError()) << endl;
                    if(exitcode == 0){
                        qDebug()<< dstpath << endl;
                        QDir().remove(dstpath+"/"+videos[0]);
                    }else{
                        qDebug() << "Process crashed!!" << endl;
                    }
                    p->deleteLater();
                });
            }else if(videos.length() > 1){
                if(directory.exists(dstpath+"/.mylist.txt")){
                    directory.remove(dstpath+"/.mylist.txt");
                }
                QFile file(dstpath+"/.mylist.txt");
                file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
                for(auto& video: videos){
//                    qDebug() << QString("file '"+dstpath+"/"+video+"'") << endl;
                    file.write(QString("file "+dstpath+"/"+video+"\n").toUtf8());
                }
                file.close();
                p->start("ffmpeg -f concat -safe 0 -i "+dstpath+"/.mylist.txt -c copy \""+dstpath+"/"+basefilename+".mp4\" -y");
                connect(p, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, [=](int exitcode){
                    qDebug() << p->readAllStandardError() << endl;
                    if(exitcode == 0){
                        for(auto& video : videos){
                            QDir().remove(dstpath+"/"+video);
                        }
                        QDir().remove(dstpath+"/.mylist.txt");
                    }else{
                        qDebug() << "Process crashed!!" << endl;
                    }
                    p->deleteLater();
                });
            }else {
                qDebug() << "Download error!" << endl;
            }
            ui->pushButton->setEnabled(true);
            ui->pushButton->setText(tr("Download"));
            ui->lineEdit->setEnabled(true);
            ui->comboBox->setEnabled(true);
            ui->statusBar->showMessage("");
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete downloader;
}

void MainWindow::showData(const QMap<QString, QString>& datalist, bool imgrequest)
{
    //set data to GUI
    ui->lineEdit_2->setText(datalist["title"]);
    ui->lineEdit_3->setText(datalist["author"]);
    ui->lineEdit_4->setText(datalist["duration"]+"秒");
    ui->lineEdit_5->setText(datalist["views"]);
    ui->lineEdit_6->setText(datalist["likes"]);
    ui->lineEdit_7->setText(datalist["favourites"]);
    ui->lineEdit_8->setText(datalist["coins"]);
    ui->lineEdit_9->setText(datalist["part"]);
    if(imgrequest){
        QNetworkAccessManager* picmanager = new QNetworkAccessManager();
        QNetworkRequest picrequest;
        picrequest.setUrl(datalist["pic"]);
        picmanager->get(picrequest);
        connect(picmanager, &QNetworkAccessManager::finished, this, [this, picmanager](QNetworkReply* reply){
            QByteArray ba = reply->readAll();
            QImage img;
            if(pixmap == nullptr){
               pixmap = new QPixmap;
            }
            img = QImage::fromData(ba);
            *pixmap = QPixmap::fromImage(img);
            ui->label_2->setPixmap(pixmap->scaled(ui->label_2->size(), Qt::AspectRatioMode::KeepAspectRatio));
            picmanager->deleteLater();
        });
    }
}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("About"));
    msgBox.setMinimumSize(QSize(400, 200));
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(tr("Author: <a href='https://github.com/pansynchronize'>X Pan</a><br/>\
                        API powered by <a href='https://github.com/Henryhaohao/Bilibili_video_download'>Henryhaohao</a>"));
    msgBox.exec();
}

Downloader::Downloader(QWidget *parent): parent(parent)
{
}

Downloader::~Downloader()
{
//    p1.wait();
//    p1.quit();
}

void Downloader::getPlayList(QString start_url, QString cid, QString quality)
{
    std::string entropy = "rbMCKn@KuamXWlPMoJGsKcbiJKUfkPF_8dABscJntvqhRSETg";
    QString appkey, sec, params;
    std::reverse(entropy.begin(), entropy.end());
    QString charlist;
    for(auto& c : entropy){
        charlist.append( QChar(c+2) );
    }
    QStringList tmp = charlist.split(":");
    appkey = tmp[0];
    sec = tmp[1];
    params = "appkey=%1&cid=%2&otype=json&qn=%3&quality=%4&type=";
    params = params.arg(appkey, cid, quality, quality);
    QByteArray chksum = QCryptographicHash::hash((params+sec).toLocal8Bit(), QCryptographicHash::Md5).toHex();
    QString url;
    QNetworkRequest request;
    //Url request
    url = "https://interface.bilibili.com/v2/playurl?%1&sign=%2";
    url = url.arg(params, QString::fromLocal8Bit(chksum));
//    qDebug() << "URL: " << url << endl;
    QNetworkAccessManager* urlManager = new QNetworkAccessManager;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36");
    request.setRawHeader("Referer", start_url.toLocal8Bit());
    urlManager->get(request);
    connect(urlManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply* r){
//        qDebug() << "Hello" << endl;
        saveUrls(r->readAll());
        emit urlAdded();
    });
}

void Downloader::getPlayLists(const QString &cid)
{
    connect(this, &Downloader::urlAdded, this, &Downloader::urlChecker);
    getPlayList(start_url, cid, "16");
    getPlayList(start_url, cid, "32");
    getPlayList(start_url, cid, "64");
    getPlayList(start_url, cid, "80");
}

void Downloader::getMetaData(QString baseurl)
{
    QString url = "https://api.bilibili.com/x/web-interface/view?aid=%1";
    bool isNum, isWrong;
    baseurl.toInt(&isNum);
    QRegExp re("https:\\/\\/www\\.bilibili\\.com\\/video\\/av\\d+$");
    if(isNum){
        url = url.arg(baseurl);
        isWrong = false;
        start_url = url;
    }else if(re.exactMatch(baseurl)){
        QRegExp idPattern("\\d+$");
        idPattern.indexIn(baseurl);
        QString id = idPattern.cap(0);
//            qDebug() << id << endl;
        url = url.arg(id);
        isWrong = false;
    }else {
        qDebug() << "Wrong website!" << endl;
        isWrong = true;
    }
    //data request
    if(!isWrong){
        start_url = url;
        QNetworkRequest request;
        dataManager = new QNetworkAccessManager(this);
        request.setUrl(url);
        dataManager->get(request);
        connect(dataManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply* reply){
           saveData(reply->readAll());
//           qDebug() << this->datalist << endl;
           emit dataUpdated(this->datalist, allpages);
           getPlayLists(datalist["page 1"]["cid"]);
        });
    }
}

void Downloader::saveData(QString pg, QString aid, QString cid, QString title, QString part,
                          QString pic, QString author, QString duration,
                          QString likes, QString favourites,
                          QString coins, QString views)
{
    QMap<QString, QString> data;
    data.insert("aid", aid);
    data.insert("cid", cid);
    data.insert("title", title);
    data.insert("part", part);
    data.insert("pic", pic);
    data.insert("author", author);
    data.insert("duration", duration);
    data.insert("likes", likes);
    data.insert("favourites", favourites);
    data.insert("coins", coins);
    data.insert("views", views);
    datalist.insert(pg, data);
}

void Downloader::saveData(const QByteArray &ba)
{
    jd = QJsonDocument::fromJson(ba);
    QJsonObject jsobj = jd.object()["data"].toObject();
    QString aid, cid, title, part, pic, author, duration, likes, favourites, coins, views, pg;
    //get data from json
    QJsonArray pages = jsobj["pages"].toArray();
    allpages.clear();
    datalist.clear();
    for(auto page = pages.begin(); page!=pages.end(); page++){
        QJsonObject pgobj = page->toObject();
        aid = QString::number(jsobj["aid"].toInt());
        cid = QString::number(pgobj["cid"].toInt());
        pg = "page "+QString::number(pgobj["page"].toInt());
        allpages.append(pg);
        duration = QString::number(pgobj["duration"].toInt());
        title = jsobj["title"].toString();
        part = pgobj["part"].toString();
        author = jsobj["owner"].toObject()["name"].toString();
        likes = QString::number(jsobj["stat"].toObject()["like"].toInt());
        favourites = QString::number(jsobj["stat"].toObject()["favorite"].toInt());
        coins = QString::number(jsobj["stat"].toObject()["coin"].toInt());
        views = QString::number(jsobj["stat"].toObject()["view"].toInt());
        pic = jsobj["pic"].toString();
        saveData(pg, aid, cid, title, part, pic, author, duration, likes,favourites, coins, views);
    }
}

QMap<QString, QMap<QString, QString>> Downloader::getData() const
{
    return datalist;
}

QMap<QString, QStringList> Downloader::getUrls() const
{
    return this->urllist;
}

void Downloader::download(const QStringList& url, QString start_url, QString path, QString pg)
{
    QString dstPath = path+"/"+datalist[pg]["title"];
//    QNetworkAccessManager* manager = new QNetworkAccessManager;
    if(!QDir(dstPath).exists()){
        QDir().mkdir(dstPath);
    }
    QList<QNetworkAccessManager*> managers;
    QList<QNetworkReply*> replies;
    QNetworkRequest request;
//    QNetworkReply* reply;
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.13; rv:56.0) Gecko/20100101 Firefox/56.0");
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("Accept-Language", "en-US,en;q=0.5");
    request.setRawHeader("Accept-Encoding", "gzip, deflate, br");
    request.setRawHeader("Range", "bytes=0-");
    request.setRawHeader("Referer", start_url.toUtf8());
    request.setRawHeader("Origin", "https://www.bilibili.com");
    request.setRawHeader("Connection", "keep-alive");

    for(uint8_t i = 0; i< url.length(); i++){
        dynamic_cast<MainWindow*>(parent)->numVideos++;
        request.setUrl(url[i]);
        managers.append(new QNetworkAccessManager());
        replies.append(managers[i]->get(request));
        qDebug() << "Downloading..."+QString::number(i) << endl;

        connect(replies[i], &QNetworkReply::downloadProgress, this, [=](qint64 received, qint64 total){
            totaldata.insert(i, total);
            receiveddata.insert(i, received);
            emit dataReceived(sum(receiveddata.values()), sum(totaldata.values()));
        });
        connect(this, &Downloader::dataReceived, dynamic_cast<MainWindow*>(parent), &MainWindow::updateProgressBar);
        connect(managers[i], &QNetworkAccessManager::finished, [=](QNetworkReply* reply){
            auto fname = ((allpages.length()==1||datalist[pg]["part"].isEmpty())?datalist[pg]["title"]:datalist[pg]["part"]);
            qDebug() << allpages.length() << endl;
            QString dstfilepath = dstPath+"/."+fname+QString::number(i)+".flv";
            QFile file(dstfilepath);
            file.open(QIODevice::WriteOnly);
            file.write(reply->readAll());
            file.close();
            qDebug() << "Downloaded!" + QString::number(i) << endl;
            dynamic_cast<MainWindow*>(parent)->numVideos--;
            emit downloaded(dstPath, fname);
            managers[i]->deleteLater();
        });
    }
}

long long Downloader::sum(const QList<long long> &list)
{
    long long tmp = 0;
    for(auto& i : list){
        tmp += i;
    }
    return tmp;
}

void Downloader::saveUrls(const QByteArray &ba)
{
    QJsonDocument jd = QJsonDocument::fromJson(ba);
    QJsonArray durl = jd.object()["durl"].toArray();
    QStringList lst;
    for(auto i = 0; i < durl.count(); i++){
        lst.append(durl[i].toObject()["url"].toString());
    }
//    qDebug() << lst << endl;
//    std::mutex mtex;
//    std::lock_guard<std::mutex> guard(mtex);
    QMap<QString, QString> map({{"16", "360P"},{"32", "480P"},{"64", "720P"}, {"80", "1080P"}});
    urllist.insert(map[QString::number(jd.object()["quality"].toInt())], lst);
}

void Downloader::urlChecker()
{
    counter++;
    if(counter >= 4){
        emit urlUpdated(this->urllist);
    }
}

void UrlReceiver::process(QString start_url, QString cid, QString quality)
{
    std::string entropy = "rbMCKn@KuamXWlPMoJGsKcbiJKUfkPF_8dABscJntvqhRSETg";
    QString appkey, sec, params;
    std::reverse(entropy.begin(), entropy.end());
    QString charlist;
    for(auto& c : entropy){
        charlist.append( QChar(c+2) );
    }
    QStringList tmp = charlist.split(":");
    appkey = tmp[0];
    sec = tmp[1];
    params = "appkey=%1&cid=%2&otype=json&qn=%3&quality=%4&type=";
    params = params.arg(appkey, cid, quality, quality);
    QByteArray chksum = QCryptographicHash::hash((params+sec).toUtf8(), QCryptographicHash::Md5).toHex();
    QString url;
    QNetworkRequest request;
    //Url request
    url = "https://interface.bilibili.com/v2/playurl?%1&sign=%2";
    url = url.arg(params, QString::fromUtf8(chksum));
//    qDebug() << "URL: " << url << endl;
    QNetworkAccessManager* urlManager = new QNetworkAccessManager;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36");
    request.setRawHeader("Referer", start_url.toUtf8());
    urlManager->get(request);
    connect(urlManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply* r){
        emit finished(r->readAll());
    });
}

void MainWindow::on_pushButton_clicked()
{
    if(path.isEmpty()){
        QString dir = QFileDialog::getExistingDirectory(this, tr("Download to"), QDir::homePath());
        if(!dir.isEmpty()){
            qDebug() << dir << endl;
            path = dir;
            settings.setValue("path", dir);
        }else {
            qDebug() << "No Path" << endl;
            return;
        }
    }
    QStringList lst;
    for(auto& key : urllist.keys()){
        lst.append(key);
    }
    sc = new SourceChooser(this);
    connect(sc, &SourceChooser::confirmed, this, [this](QString res){
        ui->pushButton->setEnabled(false);
        ui->pushButton->setText(tr("Downloading"));
        ui->lineEdit->setEnabled(false);
        ui->comboBox->setEnabled(false);
        downloader->download(urllist[res], downloader->start_url, path, ui->comboBox->currentText());
        sc->deleteLater();
    });
    sc->addSource(lst);
    sc->exec();
}

void MainWindow::on_lineEdit_returnPressed()
{
    ui->pushButton->setEnabled(false);
    ui->progressBar->setValue(0);
    ui->comboBox->setEnabled(false);
    downloader->getMetaData(ui->lineEdit->text());
//    qDebug() << ui->lineEdit->text() << endl;
}

void MainWindow::on_actionPreferences_triggered()
{
    bs = new BSettings(settings, this);
    connect(bs, &BSettings::applied, this, [=](const QMap<QString,QString>& setting){
        for(auto& k : setting.keys()){
            settings.setValue(k, setting[k]);
        }
        path = settings.value("path").toString();
        bs->deleteLater();
    });
    bs->exec();
}

void MainWindow::updateProgressBar(qint64 received, qint64 total)
{
    ui->progressBar->setValue(static_cast<int>(100*received/total));
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    ui->pushButton->setEnabled(false);
    showData(alldata[arg1]);
    downloader->getPlayLists(alldata[arg1]["cid"]);
}

void MainWindow::on_actionVA_Converter_triggered()
{
    VAConverter* vac = new VAConverter;
    vac->exec();
}
