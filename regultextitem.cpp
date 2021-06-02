#include "regultextitem.h"

///////////////////////////////////////////////////////////////////////////////////////////
RegulTextItem::RegulTextItem(QGraphicsItem *parent) : QGraphicsTextItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);

}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << Q_FUNC_INFO << pos();
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}
