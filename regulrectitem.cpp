#include "regulrectitem.h"
#include <QPen>

///////////////////////////////////////////////////////////////////////////////////////////
RegulRectItem::RegulRectItem(QGraphicsItem *parent) : QGraphicsRectItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setPen(QPen(Qt::white, 1));

}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulRectItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << Q_FUNC_INFO << pos();
    QGraphicsRectItem::mouseDoubleClickEvent(event);
}
