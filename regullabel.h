#ifndef REGULLABEL_H
#define REGULLABEL_H

#include <QLabel>
#include <QObject>
#include <QMouseEvent>

class RegulLabel : public QLabel
{
    Q_OBJECT
public:
    RegulLabel(const QString & texte = "", QWidget * parent = Q_NULLPTR);


signals:
    void clicked();

public:
    void mousePressEvent(QMouseEvent *) override;
};

#endif // REGULLABEL_H
