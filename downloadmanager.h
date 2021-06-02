#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QDir>
#include <QNetworkAccessManager>
#include <QObject>

class DownLoadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownLoadManager(const QString & path, QObject *parent = nullptr);

    void download(const QString& address, const QString &path, const QString& name);
    QDir downloadDir() const { return m_downloadDir; }

signals:
    void done();

private slots:
    void downLoadFinished();

private:
    QDir                   m_downloadDir;
    QNetworkAccessManager  m_manager;
    QNetworkReply*         m_reply;
    QDir                   m_targetDir;
    QString                m_targetName;
};

#endif // DOWNLOADMANAGER_H
