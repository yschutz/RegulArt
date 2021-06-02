#ifndef REGULPEINTRE_H
#define REGULPEINTRE_H

#include <QObject>

#include <regultableau.h>

class RegulPeintre : public QObject
{
    Q_OBJECT
public:
    explicit RegulPeintre(const QString &nom, const QString &complet, QObject *parent = nullptr);
    ~RegulPeintre();
    void                 addTableau(RegulTableau * t);
    QString              nom() const                     {return m_nom;}
    QString              nomComplet() const              {return m_nomComplet;}
    QString              photoUrl() const                {return m_photoUrl;}
    void                 setPhotoLoaded(bool val)        {m_photoLoaded = val;}
    void                 setPhotoUrl(const QString &val) {m_photoUrl = val;}
    QList<RegulTableau*> Tableaux() const                   {return m_tableaux;}

signals:

private:
    QString                 m_nom;
    QString                 m_nomComplet;
    bool                    m_photoLoaded = false;
    QString                 m_photoUrl;
    QList<RegulTableau*>    m_tableaux;
};

#endif // REGULPEINTRE_H
