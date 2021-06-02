#ifndef REGULSTREAMERS_H
#define REGULSTREAMERS_H

#include "regulellipseitem.h"
#include "regullineitem.h"

#include <QDataStream>
#include <QFont>
#include <QGraphicsLineItem>
#include <QPen>

QDataStream &operator<<(QDataStream & out, QGraphicsItem * item);
QDataStream &operator>>(QDataStream & in,  QGraphicsItem* & item);
QDataStream &operator<<(QDataStream & out, QGraphicsLineItem * item);
QDataStream &operator>>(QDataStream & in,  QGraphicsLineItem * item);
QDataStream &operator<<(QDataStream & out, QGraphicsEllipseItem * item);
QDataStream &operator>>(QDataStream & in,  QGraphicsEllipseItem * item);
QDataStream &operator<<(QDataStream & out, RegulLineItem * item);
QDataStream &operator>>(QDataStream & in,  RegulLineItem * item);
QDataStream &operator<<(QDataStream & out, RegulEllipseItem * item);
QDataStream &operator>>(QDataStream & in,  RegulEllipseItem * item);


#endif // REGULSTREAMERS_H
