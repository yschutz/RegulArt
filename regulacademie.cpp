#include "regulacademie.h"
#include "regulmainwindow.h"
#include "regultableau.h"


#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>

///////////////////////////////////////////////////////////////////////////////////////////
RegulAcademie::RegulAcademie(QObject *parent) : QObject(parent)
  ,m_jsonFilePath(":/données/tableaux.json")
{
    DownLoadManager * mgr = static_cast<RegulMainWindow*>(parent)->downLoadManager();
    browseJson();
    connect(mgr, &DownLoadManager::done,    this, &RegulAcademie::loaded);
    connect(this,   &RegulAcademie::tableauHDready, [=](){loadImage(kLD);});
    connect(this,   &RegulAcademie::tableauLDready, [=](){loadImage(kPhoto);});
}

///////////////////////////////////////////////////////////////////////////////////////////
RegulAcademie::~RegulAcademie()
{
    qDeleteAll(m_peintres.begin(), m_peintres.end());
    m_peintres.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulAcademie::browseJson()
{
    QFile dataFile(m_jsonFilePath);
    if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "fichier " << m_jsonFilePath << " non trouvé";
        exit(1);
    }
    const QString data = QString::fromUtf8(dataFile.readAll());
    dataFile.close();

    m_jsonDoc = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray peintres = m_jsonDoc.array();

    QJsonArray::Iterator ita = peintres.begin();
    while (!ita->toObject().empty()) {
        QString nom        = ita->toObject()["Auteur"].toString();
        QString nomComplet = ita->toObject()["Auteur long"].toString();
        RegulPeintre *pepe = new RegulPeintre(nom, nomComplet);
        QString photoUrl   = ita->toObject()["Photo"].toString();
        pepe->setPhotoUrl(photoUrl);
        m_peintres.append(pepe);

        QJsonArray tableaux = ita->toObject()["Tableaux"].toArray();
        QJsonArray::Iterator itt = tableaux.begin();
        while (!itt->toObject().empty()) {
            QString nom = itt->toObject()["Nom"].toString();
            RegulTableau *tata = new RegulTableau(nom);
            QString iHDUrl = itt->toObject()["UrlHD"].toString();
            tata->setImageHDUrl(iHDUrl);
            QString iLDUrl = itt->toObject()["UrlLD"].toString();
            tata->setImageLDUrl(iLDUrl);
            QJsonArray xy = itt->toObject()["Dimensions"].toArray();
            tata->setDimensions(xy[1].toDouble(), xy[0].toDouble());
            pepe->addTableau(tata);
            itt++;
        }
        ita++;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulAcademie::downLoadImagesAndPhoto()
{
    loadImage(kHD);
}

//////////////////////////////////////////////////////////////////////////////////////////
void RegulAcademie::loadImage(const ImageType type)
{
    DownLoadManager * mgr = static_cast<RegulMainWindow*>(this->parent())->downLoadManager();
    QString source;
    QString path = mgr->downloadDir().path();
    QString filePath;
    QString fileName;
        switch (type) {
    case kHD:
        source = m_peintres.at(m_peintreCurrent)->Tableaux().at(m_tableauCurrent)->imageHDUrl();
        filePath = path + "/" + m_peintres.at(m_peintreCurrent)->nom() + "/";
        fileName = m_peintres.at(m_peintreCurrent)->Tableaux().at(m_tableauCurrent)->nom();
        break;
    case kLD:
        source = m_peintres.at(m_peintreCurrent)->Tableaux().at(m_tableauCurrent)->imageLDUrl();
        filePath = path + "/" + m_peintres.at(m_peintreCurrent)->nom() + "/";
        fileName = m_peintres.at(m_peintreCurrent)->Tableaux().at(m_tableauCurrent)->nom() +"_LD";
        break;
    case kPhoto:
        source = m_peintres.at(m_peintreCurrent)->photoUrl();
        filePath = path + "/";
        fileName = m_peintres.at(m_peintreCurrent)->nom();
        break;
    }
    QString ext = source.right(source.size() - source.lastIndexOf("."));
    fileName = fileName + ext;
    if (!QFileInfo::exists(filePath + fileName))
        mgr->download(source, filePath, fileName);
}

//////////////////////////////////////////////////////////////////////////////////////////
void RegulAcademie::loaded()
{
    if (!m_imageHDLoaded) {
        m_imageHDLoaded = true;
        emit tableauHDready();
    } else if (!m_imageLDLoaded) {
        m_imageLDLoaded = true;
        emit tableauLDready();
    } else if (!m_photoLoaded) {
        m_photoLoaded = true;
        emit photoready();
        m_imageHDLoaded = m_imageLDLoaded = m_photoLoaded = false;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
QString RegulAcademie::nomComplet(const QString &nom)
{
    QList<RegulPeintre*>::iterator itp;
    for (itp = m_peintres.begin(); itp != m_peintres.end(); itp++)
        if ((*itp)->nom() == nom)
            break;
    return (*itp)->nomComplet();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulAcademie::setCurrent(const QString &name)
{
    QString peintreCurrent = name.split(":").at(0);
    QString tableauCurrent = name.split(":").at(1);
    QList<RegulPeintre*>::iterator itp;
    for (itp = m_peintres.begin(); itp != m_peintres.end(); itp++)
        if ((*itp)->nom() == peintreCurrent) {
            m_peintreCurrent =  m_peintres.indexOf(*itp);
            break;
        }

    QList<RegulTableau*> tata = (*itp)->Tableaux();
    QList<RegulTableau*>::iterator itt;
    for (itt = tata.begin(); itt != tata.end(); itt++)
        if ((*itt)->nom() == tableauCurrent) {
            m_tableauCurrent = tata.indexOf(*itt);
            break;
        }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulAcademie::setImagesAndPhotoLoaded(bool val)
{
    QString peintreNom = m_peintres.at(m_peintreCurrent)->nom();
    QList<RegulPeintre*>::iterator itp;
    for (itp = m_peintres.begin(); itp != m_peintres.end(); itp++)
        if ((*itp)->nom() == peintreNom) {
            (*itp)->setPhotoLoaded(val);
            break;
        }

    QList<RegulTableau*> tata = (*itp)->Tableaux();
    QString tableauNom = (*itp)->Tableaux().at(m_tableauCurrent)->nom();
    QList<RegulTableau*>::iterator itt;
    for (itt = tata.begin(); itt != tata.end(); itt++)
        if ((*itt)->nom() == tableauNom) {
            (*itt)->setImagesLoaded(val);
            break;
        }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulAcademie::tailleTableau(const QString &peintre, const QString &nom, qreal &w, qreal &h)
{
    QList<RegulPeintre*>::iterator itp;
    for (itp = m_peintres.begin(); itp != m_peintres.end(); itp++)
        if ((*itp)->nom() == peintre) {
            m_peintreCurrent =  m_peintres.indexOf(*itp);
            break;
        }

    QList<RegulTableau*> tata = (*itp)->Tableaux();
    QList<RegulTableau*>::iterator itt;
    for (itt = tata.begin(); itt != tata.end(); itt++)
        if ((*itt)->nom() == nom) {
            break;
        }
    w = (*itt)->width();
    h = (*itt)->height();
}
