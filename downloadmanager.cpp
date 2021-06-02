#include "downloadmanager.h"

#include <QDir>
#include <QFile>
#include <QNetworkReply>

///////////////////////////////////////////////////////////////////////////////////////////
DownLoadManager::DownLoadManager(const QString &path, QObject *parent) : QObject(parent)
{
//    m_downloadDir.setPath(QDir::homePath());
    m_downloadDir.setPath(QDir::homePath() + path + "/images/");
    if (!m_downloadDir.exists())
        m_downloadDir.mkpath(".");
}

///////////////////////////////////////////////////////////////////////////////////////////
void DownLoadManager::download(const QString &address, const QString &path, const QString &name)
{
    m_targetName = name;
    m_targetDir.setPath(path);
    QUrl url = QUrl::fromEncoded(address.toLocal8Bit());
    QNetworkRequest request(url);
    m_reply = m_manager.get(request);
    connect(m_reply, &QNetworkReply::finished, this, &DownLoadManager::downLoadFinished);
}

///////////////////////////////////////////////////////////////////////////////////////////
void DownLoadManager::downLoadFinished()
{
    if (!m_targetDir.exists())
        m_targetDir.mkpath(".");
    QString fileOutName = m_targetDir.path() + "/" + m_targetName;
    QFile fileOut(fileOutName);

    if (!fileOut.open(QIODevice::WriteOnly)) {
        qWarning() << "Impossible de créer le fichier " << fileOutName;
        exit(1);
    }
    fileOut.write(m_reply->readAll());
    fileOut.close();
    m_reply->deleteLater();
    emit done();
}

