#include "regulellipseitem.h"
#include "regulimager.h"
#include "regulscene.h"
#include "regultoolbox.h"
#include "regulviewer.h"

#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

///////////////////////////////////////////////////////////////////////////////////////////
RegulEllipseItem::RegulEllipseItem(QGraphicsItem *parent) : QGraphicsEllipseItem(parent)
{
    init();
}

///////////////////////////////////////////////////////////////////////////////////////////
RegulEllipseItem::RegulEllipseItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) :
    QGraphicsEllipseItem(x, y, w, h, parent)
{
    init();
}

///////////////////////////////////////////////////////////////////////////////////////////
QList<QPointF> RegulEllipseItem::intersects()
{
    QList<QPointF> rv;
    qreal a, b, c;
    qreal res1, res2;
    QPointF tsize = static_cast<RegulScene*>(scene())->tabSize();
    QPointF center =  mapToScene(rect().center());
    QPointF rayon  =  radius();
    // x = 0
    a = 1;
    b = - 2 * center.y();
    c = center.x() * center.x() + center.y() * center.y() - rayon.x() * rayon.y();
    solve(a, b, c, res1, res2);
    if (res1 > 0 && (res1 < tsize.y())) rv.append(QPointF(0, res1));
    if (res2 > 0 && (res2 < tsize.y())) rv.append(QPointF(0, res2));
    // x = tsize.x()
    c += tsize.x() * tsize.x() - 2 * tsize.x() * center.x();
    solve(a, b, c, res1, res2);
    if (res1 > 0 && (res1 < tsize.y())) rv.append(QPointF(tsize.x(), res1));
    if (res2 > 0 && (res2 < tsize.y())) rv.append(QPointF(tsize.x(), res2));
    // y = 0
    a = 1;
    b = - 2 * center.x();
    c = center.y() * center.y() + center.x() * center.x() - rayon.x() * rayon.y();
    solve(a, b, c, res1, res2);
    if (res1 > 0 && (res1 < tsize.x())) rv.append(QPointF(res1, 0));
    if (res2 > 0 && (res2 < tsize.x())) rv.append(QPointF(res2, 0));
    // y = tsize.y()
    c += tsize.y() * tsize.y() - 2 * tsize.y() * center.y();
    solve(a, b, c, res1, res2);
    if (res1 > 0 && (res1 < tsize.x())) rv.append(QPointF(res1, tsize.y()));
    if (res2 > 0 && (res2 < tsize.x())) rv.append(QPointF(res2, tsize.y()));

    return rv;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool RegulEllipseItem::isInsideTab() const
{
   bool rv = true;
   QPointF tsize = static_cast<RegulScene*>(scene())->tabSize();
   QRectF  tRect(0, 0, tsize.x(), tsize.y());
   QPolygonF poly = mapToScene(rect());
   for (QPointF p : poly){
       if (!tRect.contains(p))
           rv = false;
   }
   return rv;
}

//////////////////////////////////////////////////////////////////////////////////////////
void RegulEllipseItem::setClip(bool test)
{
    if (test) {
        QPointF tsize = static_cast<RegulScene*>(scene())->tabSize();
        QPainterPath path;
        path.addRect(QRectF(0, 0, tsize.x(), tsize.y()));
        m_boundaryPath = path;
    }
    else
        m_boundaryPath.clear();
    update();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulEllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(!m_boundaryPath.isEmpty()){
        QPainterPath path = mapFromScene(m_boundaryPath);
        if(!path.isEmpty())
            painter->setClipPath(path);
    }
    QGraphicsEllipseItem::paint(painter, option, widget);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulEllipseItem::setBoundaryPath(bool val)
{
    if (val) {
        QPointF tsize = static_cast<RegulScene*>(scene())->tabSize();
        QPainterPath path;
        path.addRect(QRectF(0, 0, tsize.x(), tsize.y()));
        m_boundaryPath = path;
    }
    else
        m_boundaryPath.clear();
    update();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulEllipseItem::init()
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
    m_grayPen.setBrush(Qt::lightGray);
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

    m_currentPen = m_grayPen;

    setPen(m_currentPen);
}
///////////////////////////////////////////////////////////////////////////////////////////
void RegulEllipseItem::solve(qreal a, qreal b, qreal c, qreal &res1, qreal &res2)
{
    qreal delta = b * b - 4 * a * c;
    res1 = res2 = -1;
    if (delta > 0) {
        res1 = (-b + qSqrt(delta)) / (2 * a);
        res2 = (-b - qSqrt(delta)) / (2 * a);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulEllipseItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QPen mypen = pen();
    mypen.setBrush(Qt::red);
    setPen(mypen);
    if (m_handDrawn)
        updateSceneInfo(true);
    QGraphicsEllipseItem::hoverEnterEvent(event);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulEllipseItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!isSelected()) {
        QPen mypen = pen();
        QBrush mybrush = m_currentPen.brush();
        mypen.setBrush(mybrush);
        setPen(mypen);
    }
    if (m_handDrawn)
        updateSceneInfo(false);
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}

///////////////////////////////////////////////////////////////////////////////////////////
QVariant RegulEllipseItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemScenePositionHasChanged)
        updateSceneInfo();
    return QGraphicsItem::itemChange(change, value);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulEllipseItem::keyPressEvent(QKeyEvent *event)
{
    if (isSelected()) {
        RegulScene * scene = dynamic_cast<RegulScene*>(this->scene());
        setTransformOriginPoint(mapFromScene(QPointF(m_center.x() * scene->tabSize().x(),
                                                     m_center.y() * scene->tabSize().y())));
        qreal scalefactorUp   = 1.1;
        qreal scalefactorDown = 0.9;
        if (event->modifiers() == (Qt::ShiftModifier|Qt::KeypadModifier)) {
            scalefactorUp   = 1.01;
            scalefactorDown = 0.1;
        }
        if(event->key() == 43) {
            setScale(m_scale *= scalefactorUp);
        }
        else if (event->key() == 45) {
            setScale(m_scale *= scalefactorDown);
        }
        updateSceneInfo();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulEllipseItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent */*event*/)
{
    int rv = m_msg.exec();
    if (rv == QMessageBox::Yes)
        dynamic_cast<RegulScene*>(scene())->removeEllipse(this);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulEllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
        updateSceneInfo();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulEllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent */*event*/)
{
    updateSceneInfo();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulEllipseItem::setHandDrawn(bool val)
{
    m_handDrawn = val;
    if (m_handDrawn) {
        m_currentPen = pen();
        m_currentPen.setBrush(Qt::cyan);
        setPen(m_currentPen);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulEllipseItem::updateSceneInfo(bool show)
{    
    RegulScene * scene = dynamic_cast<RegulScene*>(this->scene());

    QRectF ellipseRect = mapRectToScene(rect());
    m_center = ellipseRect.center();
    m_center.setX(m_center.x() / scene->tabSize().x());
    m_center.setY(m_center.y() / scene->tabSize().y());

    qreal rw = m_scale * rect().width()  / 2. / scene->tabSize().x();
    qreal rh = m_scale * rect().height() / 2. / scene->tabSize().y();
    m_radius = QPointF(rect().width() / 2.0, rect().height() / 2.0);

    scene->viewer()->setCercleRadius(QPointF(rw, rh), show);
    scene->viewer()->setCercleCenter(m_center, show);
    if (scene->viewer()->imager()->isToolBoxOn())
        scene->viewer()->imager()->theToolBox()->setEllipse(this);
}

