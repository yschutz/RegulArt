#ifndef REGULPUSHBUTTON_H
#define REGULPUSHBUTTON_H

#include <QPushButton>
#include <QObject>

class RegulPushButton : public QPushButton
{
    Q_OBJECT
public:
    RegulPushButton(QWidget * parent = Q_NULLPTR);

    QString imageFileName() const                    {return m_imageFileName;}
    void    setImageFileName(const QString & fileName);

private:
    QString m_imageFileName;
};

#endif // REGULPUSHBUTTON_H
