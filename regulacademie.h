#ifndef REGULACADEMIE_H
#define REGULACADEMIE_H

#include <QJsonDocument>
#include <QObject>

#include "regulpeintre.h"
#include "downloadmanager.h"

class RegulAcademie : public QObject
{
    Q_OBJECT
    Q_ENUMS(ImageType)

public:
    enum ImageType {kHD, kLD, kPhoto};

    explicit RegulAcademie(QObject *parent = nullptr);
    ~RegulAcademie();

    void                 downLoadImagesAndPhoto();
    QString              nomComplet(const QString &nom);
    QList<RegulPeintre*> peintres() const                     {return m_peintres;}
    QString              peintreCourant() const               {return m_peintres.at(m_peintreCurrent)->nom();}
    void                 setCurrent(const QString &name);
    void                 setImagesAndPhotoLoaded(bool val);
    void                 setJsonFilePath(const QString &path) {m_jsonFilePath = path;}
    QString              tableauCourant() const               {return m_peintres.at(m_peintreCurrent)->Tableaux().at(m_tableauCurrent)->nom();}
    void                 tailleTableau(const QString &peintre, const QString &nom, qreal &w, qreal &h);

public slots:
    void loadImage(const ImageType type);
    void loaded();

signals:
    void photoready();
    void tableauHDready();
    void tableauLDready();

private:
    void browseJson();

    bool                 m_imageHDLoaded   = false;
    bool                 m_imageLDLoaded   = false;
    QJsonDocument        m_jsonDoc;
    QString              m_jsonFilePath;
    QList<RegulPeintre*> m_peintres;
    int                  m_peintreCurrent;
    bool                 m_photoLoaded     = false;
    int                  m_tableauCurrent;
};

#endif // REGULACADEMIE_H
