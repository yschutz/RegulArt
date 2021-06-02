#include "regulimager.h"
#include "regullineitem.h"
#include "regulscene.h"
#include "regultoolbox.h"
#include "regulviewer.h"

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QPen>

///////////////////////////////////////////////////////////////////////////////////////////
RegulLineItem::RegulLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent) :
    QGraphicsLineItem(x1, y1, x2, y2, parent)
{
    init();
}

///////////////////////////////////////////////////////////////////////////////////////////
RegulLineItem::RegulLineItem(qreal x1, qreal y1, qreal x2, qreal y2, const QPen &pen, QGraphicsItem *parent) :
 QGraphicsLineItem(x1, y1, x2, y2, parent)
{
    init();
    m_currentPen = pen;
    setPen(m_currentPen);
}

///////////////////////////////////////////////////////////////////////////////////////////
RegulLineItem::RegulLineItem(QGraphicsItem *parent) :
    QGraphicsLineItem(parent)
{
    init();

    m_currentPen = m_grayPen;
    setPen(m_currentPen);
}

///////////////////////////////////////////////////////////////////////////////////////////
QPointF RegulLineItem::equation() const
{
    QPointF rv;
    qreal a = (m_beg.y() - m_end.y()) / (m_beg.x() - m_end.y());
    qreal b = m_beg.y() - a * m_beg.x();
    rv.setX(a);
    rv.setY(b);
    return rv;
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulLineItem::init()
{
    m_msg.setWindowTitle("Tracé Manuel");
    m_msg.setText("Effacer ce tracé ?");
    m_msg.setStandardButtons(QMessageBox::Yes);
    m_msg.setButtonText(QMessageBox::Yes, "Oui");
    m_msg.addButton(QMessageBox::No);
    m_msg.setButtonText(QMessageBox::No, "Non");
    m_msg.setDefaultButton(QMessageBox::No);

    m_grayPen.setStyle(Qt::SolidLine);
    m_grayPen.setWidth(2);
    m_grayPen.setBrush(Qt::white);
    setPen(m_grayPen);

    m_redPen.setStyle(Qt::SolidLine);
    m_redPen.setWidth(2);
    m_redPen.setBrush(Qt::red);

    m_cyanPen.setStyle(Qt::SolidLine);
    m_cyanPen.setWidth(2);
    m_cyanPen.setBrush(Qt::cyan);

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable |
             QGraphicsItem::ItemSendsScenePositionChanges);
    setAcceptHoverEvents(true);

}


///////////////////////////////////////////////////////////////////////////////////////////
void RegulLineItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
   QPen mypen = pen();
   mypen.setBrush(Qt::red);
   setPen(mypen);
    if (m_handDrawn) {
        qreal endX = line().x1() + line().dx();
        qreal endY = line().x2() + line().dy();
        m_end.setX(mapToScene(endX, endY).x());
        m_end.setY(mapToScene(endX, endY).y());
        updateSceneInfo(true);
    }
    QGraphicsLineItem::hoverEnterEvent(event);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulLineItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!isSelected()) {
        QPen mypen = pen();
        QBrush mybrush = m_currentPen.brush();
        mypen.setBrush(mybrush);
        setPen(mypen);
    }
    if (m_handDrawn)
        updateSceneInfo(false);
    QGraphicsLineItem::hoverLeaveEvent(event);
}

///////////////////////////////////////////////////////////////////////////////////////////
QVariant RegulLineItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemScenePositionHasChanged)
        updateSceneInfo();
    return QGraphicsItem::itemChange(change, value);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulLineItem::keyPressEvent(QKeyEvent *event)
{
    if (isSelected()) {
        if(event->key() == 43) {
            if (event->modifiers() == (Qt::ShiftModifier|Qt::KeypadModifier))
                setScale(m_scale *= 1.1);
            else
                setRotation(m_rotate +=1);
        }
        else if (event->key() == 45) {
            if (event->modifiers() == (Qt::ShiftModifier|Qt::KeypadModifier))
                setScale(m_scale *= 0.9);
            else
                setRotation(m_rotate -=1);
        }
        updateSceneInfo();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulLineItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent */*event*/)
{
    int rv = m_msg.exec();
    if (rv == QMessageBox::Yes)
        dynamic_cast<RegulScene*>(scene())->removeLigne(this);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << Q_FUNC_INFO;
    if(event->button() == Qt::RightButton)
        updateSceneInfo();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulLineItem::setHandDrawn(bool val)
{
    m_handDrawn = val;
    if (m_handDrawn) {
        m_currentPen = pen();
        m_currentPen.setBrush(Qt::cyan);
        setPen(m_currentPen);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulLineItem::setMyLine(qreal x1, qreal y1, qreal x2, qreal y2)
{
    m_beg.setX(mapToScene(x1, y1).x());
    m_beg.setY(mapToScene(x1, y1).y());
    m_end.setX(mapToScene(x2, y2).x());
    m_end.setY(mapToScene(x2, y2).y());
    setLine(x1, y1, x2, y2);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulLineItem::updateSceneInfo(bool show)
{
    RegulScene * scene = dynamic_cast<RegulScene*>(this->scene());
    m_beg = mapToScene(line().x1(), line().y1());
    m_end = mapToScene(line().x2(), line().y2());


    scene->viewer()->setLigneBeg(QPointF(m_beg.x() / scene->tabSize().x(),
                                         m_beg.y() / scene->tabSize().y()), show);
    scene->viewer()->setLigneEnd(QPointF(m_end.x() / scene->tabSize().x(),
                                         m_end.y() / scene->tabSize().y()), show);

    if (scene->viewer()->imager()->isToolBoxOn()) {
        if (show)
            scene->viewer()->imager()->theToolBox()->setLine(this);
    }
}
