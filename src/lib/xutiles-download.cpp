 
#include "xutiles-download.hpp"

xdr::xDownload::xDownload(QObject *parent) : QObject(parent)
{
}

void xdr::xDownload::download(QUrl url, QString folder) {
    if (folder.isEmpty() || url.isEmpty())
    {
        return;
    }
    _file = new QFile(folder + QDir::separator() + url.fileName());

    if (!_file->open(QIODevice::WriteOnly))
        {
            delete _file;
            _file = nullptr;
            return;
        }

    QNetworkRequest request(url);

    reply = m_WebCtrl.get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(onDownloadReady()));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(getDownloadData(qint64, qint64)) );

    connect(reply, SIGNAL(finished()),this, SLOT(fileDownloaded()));
}

void xdr::xDownload::fileDownloaded()
{
    m_DownloadedData = reply->readAll();
    reply->deleteLater();
    QFileInfo info = QFileInfo(*_file);
    emit downloaded(info.absoluteFilePath());
}

QByteArray xdr::xDownload::downloadedData() const
{
    return m_DownloadedData;
}

void xdr::xDownload::onDownloadReady() {
    if (_file)
    {
        _file->write(reply->readAll());
    }
}

 void xdr::xDownload::getDownloadData(qint64 read, qint64 total)
 {

     percent = ((double)read / (double)total) * 100;
     emit percentDownloaded(percent);
 }

 void xdr::xDownload::installFile(QString filepath) {
    auto proc = QProcess();
    proc.start(filepath, QStringList() << "--help");
    proc.waitForFinished(-1);
    QString output = QString(proc.readAllStandardOutput());
    std::cout << output.toStdString();
    output = QString(proc.readAllStandardError());
    std::cout << output.toStdString();
 }

 xdr::xDownload::~xDownload() {

 }