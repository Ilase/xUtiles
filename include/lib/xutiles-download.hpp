#ifndef XDOWNLOAD_H
#define XDOWNLOAD_H

#include <QObject>
#include <QByteArray>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QFile>
#include <QProcess>
#include <QDir>
#include <iostream>

namespace xdr {

    class xDownload : public QObject
    {
        Q_OBJECT
    public:
        explicit xDownload(QObject *parent = nullptr);
        virtual ~xDownload();
        QByteArray downloadedData() const;
        void download(QUrl, QString);

    signals:
            void downloaded(QString);
            void percentDownloaded(int);
    public  slots:
            void installFile(QString file);
    private slots:
        void fileDownloaded();
        void  getDownloadData(qint64 read, qint64 total);
        void onDownloadReady();
    private:
        int percent;
        QProcess process;
        QFile* _file = {nullptr};
        QNetworkAccessManager m_WebCtrl;
        QByteArray m_DownloadedData;
        QNetworkReply* reply;
    };

}

#endif // XDOWNLOAD_H

