#ifndef REGULRECTITEM_H
#define REGULRECTITEM_H

#include <QGraphicsRectItem>

class RegulRectItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    RegulRectItem(QGraphicsItem *parent = nullptr);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

};

#endif // REGULRECTITEM_H
