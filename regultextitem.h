#ifndef REGULTEXTITEM_H
#define REGULTEXTITEM_H

#include <QGraphicsTextItem>

class RegulTextItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    RegulTextItem(QGraphicsItem *parent = nullptr);

signals:

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

};

#endif // REGULTEXTITEM_H
