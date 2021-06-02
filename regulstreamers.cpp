#include "regulstreamers.h"

///////////////////////////////////////////////////////////////////////////////////////////
QDataStream &operator<<(QDataStream & out, QGraphicsItem * item){
    out << item->pos()
        << item->scale()
        << item->rotation()
        << item->transform()
        << item->transformOriginPoint()
        << item->flags()
        << item->isEnabled()
        << item->isSelected()
        << item->zValue();
    return out;
}

///////////////////////////////////////////////////////////////////////////////////////////
QDataStream &operator>>(QDataStream & in, QGraphicsItem* & item){
    QPointF pos;
    qreal scale;
    qreal rotation;
    QTransform transform;
    QPointF transformOriginPoint;
    QGraphicsItem::GraphicsItemFlags flags;
    bool isEnabled;
    bool isSelected;
    qreal zValue;
    in >> pos
            >> scale
            >> rotation
            >> transform
            >> transformOriginPoint
            >> flags
            >> isEnabled
            >> isSelected
            >> zValue;
    item->setPos(pos);
    item->setScale(scale);
    item->setRotation(rotation);
    item->setTransform(transform);
    item->setTransformOriginPoint(transformOriginPoint);
    item->setFlags(flags);
    item->setEnabled(isEnabled);
    item->setSelected(isSelected);
    item->setZValue(zValue);
    return in;
}

///////////////////////////////////////////////////////////////////////////////////////////
QDataStream &operator<<(QDataStream & out, QGraphicsLineItem * item){
    out << dynamic_cast<QGraphicsItem *>(item);
    out << item->pen() << item->line();
    return out;
}

///////////////////////////////////////////////////////////////////////////////////////////
QDataStream &operator>>(QDataStream & in, QGraphicsLineItem * item){
    QGraphicsItem *tmp = dynamic_cast<QGraphicsItem *>(item);
    in >> tmp;

    QPen pen;
    QLineF line;
    in >> pen >> line;

    item->setPen(pen);
    item->setLine(line);
    return in;
}

///////////////////////////////////////////////////////////////////////////////////////////
QDataStream &operator<<(QDataStream & out, QGraphicsEllipseItem * item)
{
    out << dynamic_cast<QGraphicsItem *>(item);
    out << item->rect() << item->startAngle() << item->spanAngle();

    return out;
}

///////////////////////////////////////////////////////////////////////////////////////////
QDataStream &operator>>(QDataStream & in, QGraphicsEllipseItem * item)
{
    QGraphicsItem *tmp = dynamic_cast<QGraphicsItem *>(item);
    in >> tmp;

    QRectF rect;
    int startAngle;
    int spanAngle;

    in >> rect >> startAngle >> spanAngle;

    item->setRect(rect);
    item->setStartAngle(startAngle);
    item->setSpanAngle(spanAngle);

    return in;
}

///////////////////////////////////////////////////////////////////////////////////////////
QDataStream &operator<<(QDataStream & out, RegulLineItem * item)
{
    out << dynamic_cast<QGraphicsItem *>(item);
    out << item->pen() << item->line() << item->isHandDrawn();

    return out;
}

///////////////////////////////////////////////////////////////////////////////////////////
QDataStream &operator>>(QDataStream & in, RegulLineItem * item)
{
    QGraphicsItem *tmp = dynamic_cast<QGraphicsItem *>(item);
    in >> tmp;

    QPen   pen;
    QLineF line;
    bool   handDrawn;
    in >> pen >> line >> handDrawn;

    item->setPen(pen);
    item->setLine(line);
    item->setHandDrawn(handDrawn);

    return in;
}

///////////////////////////////////////////////////////////////////////////////////////////
QDataStream &operator<<(QDataStream & out, RegulEllipseItem * item)
{
    out << dynamic_cast<QGraphicsItem *>(item);
    out << item->pen() <<  item->rect() << item->startAngle() << item->spanAngle();
    out << item->radius() << item->center() << item->currentPen();

    return out;
}

///////////////////////////////////////////////////////////////////////////////////////////
QDataStream &operator>>(QDataStream & in, RegulEllipseItem * item)
{
    QGraphicsItem *tmp = dynamic_cast<QGraphicsItem *>(item);
    in >> tmp;

    QPen    pen;
    QPen    cpen;
    QRectF  rect;
    int     startAngle;
    int     spanAngle;
    QPointF radius;
    QPointF center;

    in >> pen >> rect >> startAngle >> spanAngle >> radius >> center >> cpen;

    item->setPen(pen);
    item->setRect(rect);
    item->setStartAngle(startAngle);
    item->setSpanAngle(spanAngle);
    item->setCenter(center);
    item->setRadius(radius);
    item->setCurrentPen(cpen);

    return in;
}
