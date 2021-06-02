#ifndef REGULTABLEAU_H
#define REGULTABLEAU_H

#include <QObject>

class RegulTableau : public QObject
{
    Q_OBJECT
public:
    explicit RegulTableau(const QString &nom, QObject *parent = nullptr);

    qreal   height() const                    {return m_height;}
    QString imageHDUrl() const                {return m_imageHDUrl;}
    QString imageLDUrl() const                {return m_imageLDUrl;}
    QString nom() const                       {return m_nom;}
    void    setImageHDUrl(const QString &val) {m_imageHDUrl = val;}
    void    setImageLDUrl(const QString &val) {m_imageLDUrl = val;}
    void    setImagesLoaded(bool val)         {m_imageHDLoaded = val; m_imageLDLoaded = val;}
    void    setDimensions(qreal x, qreal y)   {m_width = x; m_height = y;}
    qreal   width() const                     {return m_width;}


private:
    QString m_nom;
    qreal   m_height;
    QString m_imageHDUrl;
    QString m_imageLDUrl;
    qreal   m_width;
    bool    m_imageHDLoaded = false;
    bool    m_imageLDLoaded = false;
};

#endif // REGULTABLEAU_H
