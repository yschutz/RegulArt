#include "regulitemgroup.h"
#include "regulellipseitem.h"
#include "regullineitem.h"
#include "regulrectitem.h"
#include "regulscene.h"
#include "regultextitem.h"
#include "regulviewer.h"

#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>

///////////////////////////////////////////////////////////////////////////////////////////
RegulScene::RegulScene(QObject *parent) : QGraphicsScene(parent)
  , m_carreaux(nullptr), m_crossHairH(nullptr), m_crossHairV(nullptr),
    m_diagonales(nullptr), m_handDrawnLine(nullptr), m_image(nullptr)
{
    m_msg.setWindowTitle(tr("Tracé Manuel"));
    m_msg.setStandardButtons(QMessageBox::Yes);
    m_msg.setButtonText(QMessageBox::Yes, "Oui");
    m_msg.addButton(QMessageBox::No);
    m_msg.setButtonText(QMessageBox::No, "Non");
    m_msg.setDefaultButton(QMessageBox::No);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::addBoundingBox(qreal w, qreal h)
{
    RegulRectItem *rectItem = new RegulRectItem();
    rectItem->setRect(0, 0, w, h);
    QPen pen = rectItem->pen();
    pen.setWidth(10);
    rectItem->setPen(pen);
    rectItem->setFlag(QGraphicsItem::ItemIsMovable, false);
    addItem(rectItem);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::addCarreaux(int nw, int nh)
{
    if(!m_carreaux) {
        m_carreaux = new RegulItemGroup();
        m_carreaux->makeCarreaux(m_tabSize, nw, nh);
        for (QGraphicsLineItem* line : m_carreaux->list()){
            addItem(line);
            line->show();
        }
    } else
        for (QGraphicsLineItem* line : m_carreaux->list())
            line->show();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::addCrossHair()
{
    if (m_crossHairH == nullptr || m_crossHairV == nullptr) {
        QPen pen;
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(1);
        pen.setBrush(Qt::white);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);

        qreal y = m_tabSize.y() / 2.;
        qreal x = m_tabSize.x();
        m_crossHairH = addLine(0.0, y, x, y, pen);
        y = m_tabSize.y();
        x = m_tabSize.x() / 2.;
        m_crossHairV = addLine(x, 0.0, x, y, pen);
    } else {
        m_crossHairH->show();
        m_crossHairV->show();
    }
    m_crossHairOn = true;
    setCrossHairCenter(QPointF(m_tabSize.x() / 2., m_tabSize.y() / 2.));
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::addDiagonales()
{
    if(!m_diagonales) {
        m_diagonales = new RegulItemGroup();
        m_diagonales->makeDiagonales(m_tabSize);
        for (QGraphicsLineItem* line : m_diagonales->list()){
            addItem(line);
            line->show();
        }
    } else
        for (QGraphicsLineItem* line : m_diagonales->list())
            line->show();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::addHandDrawnEllipse(RegulEllipseItem *item)
{
    m_handDrawnEllipses.append(item);

}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::addHandDrawnLine(RegulLineItem *ligne)
{
    m_handDrawnLines.append(ligne);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::addImage(const QImage &image)
{
    if(m_image != nullptr)
        removeItem(m_image);
    m_image = addPixmap(QPixmap::fromImage(image));
    m_image->setZValue(-1);
    m_viewer = dynamic_cast<RegulViewer*>(views().at(0));
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::addMyText(const QString &text, QPointF pos)
{
    RegulTextItem *textItem = new RegulTextItem();
    textItem->setPlainText(text);
    addItem(textItem);
    RegulRectItem *rectItem = new RegulRectItem();
    rectItem->setRect(pos.x(), pos.y(), 100, 100);
    addItem(rectItem);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::hideAllHanDrawn()
{
    QList<QGraphicsEllipseItem*>::iterator ite;
    for (ite = m_handDrawnEllipses.begin(); ite != m_handDrawnEllipses.end(); ++ite)
        (*ite)->hide();

    QList<QGraphicsLineItem*>::iterator itl;
    for (itl = m_handDrawnLines.begin(); itl != m_handDrawnLines.end(); ++itl)
        (*itl)->hide();

}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::hideCarreau()
{
    for (QGraphicsLineItem* line : m_carreaux->list())
        line->hide();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::hideCrossHair()
{
    m_crossHairH->hide();
    m_crossHairV->hide();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::hideDiagonales()
{
    for (QGraphicsLineItem* line : m_diagonales->list())
        line->hide();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::hideHandDrawnEllispe()
{
    m_handDrawingEllipseOn = false;
    hideCrossHair();
    QList<QGraphicsEllipseItem*>::iterator it;
    for (it = m_handDrawnEllipses.begin(); it != m_handDrawnEllipses.end(); ++it)
        (*it)->hide();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::hideHandDrawnLine()
{
    m_handDrawingLineOn = false;
    hideCrossHair();
    QList<QGraphicsLineItem*>::iterator it;
    for (it = m_handDrawnLines.begin(); it != m_handDrawnLines.end(); ++it)
        (*it)->hide();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::init()
{
  qDeleteAll(items());
  m_handDrawnEllipses.clear();
  m_handDrawnLines.clear();

  m_carreaux         = nullptr;
  m_crossHairH       = nullptr;
  m_crossHairV       = nullptr;
  m_diagonales       = nullptr;
  m_handDrawnEllipse = nullptr;
  m_handDrawnLine    = nullptr;

  m_handDrawnEllipseOrigine.setX(0.0);
  m_handDrawnEllipseOrigine.setY(0.0);

  m_handDrawnLineOrigine.setX(0.0);
  m_handDrawnLineOrigine.setY(0.0);


  m_image = nullptr;

  m_tabSize.setX(0.0);
  m_tabSize.setY(0.0);

  m_crossHairOn = false;
  m_handDrawingEllipseOn = m_handDrawingLineOn = false;
  m_handDrawnNewEllipse = m_handDrawnNewLine = true;
  m_mousePressed = false;
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_handDrawingLineOn) {
        m_handDrawnLineOrigine = event->scenePos();
        m_handDrawnNewLine = true;
        QPointF beg(m_handDrawnLineOrigine.x() / m_tabSize.x(), m_handDrawnLineOrigine.y() / m_tabSize.y());
        m_viewer->setLigneBeg(beg);

    } else if(m_handDrawingEllipseOn) {
        m_handDrawnEllipseOrigine = event->scenePos();
        m_handDrawnNewEllipse = true;
        QPointF center(m_handDrawnEllipseOrigine.x() / m_tabSize.x(), m_handDrawnEllipseOrigine.y() / m_tabSize.y());
        m_viewer->setCercleCenter(center);
    } else
        QGraphicsScene::mousePressEvent(event);
    m_mousePressed = true;

}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_crossHairOn) {
        qreal x = event->scenePos().x();
        qreal y = event->scenePos().y();
        setCrossHairCenter(QPointF(x,y));
        m_viewer->setMousePosVal(x / m_tabSize.x(), y / m_tabSize.y());
    }
    if (m_handDrawingLineOn && m_mousePressed) {
        if (m_handDrawnNewLine) {
            m_handDrawnLine = new RegulLineItem;
            m_handDrawnLine->setHandDrawn(true);
            addItem(m_handDrawnLine);
            m_handDrawnNewLine = false;
        }
        m_handDrawnLine->setPos(m_handDrawnLineOrigine);
        qreal x = event->scenePos().x();
        qreal y = event->scenePos().y();
        m_handDrawnLine->setMyLine(0,0, x - m_handDrawnLineOrigine.x(),
                                      y - m_handDrawnLineOrigine.y());
        m_viewer->setLigneEnd(QPointF(x / m_tabSize.x(), y  /m_tabSize.y()));
    } else if (m_handDrawingEllipseOn && m_mousePressed) {
        if (m_handDrawnNewEllipse) {
            m_handDrawnEllipse = new RegulEllipseItem;
            m_handDrawnEllipse->setHandDrawn(true);
            addItem(m_handDrawnEllipse);
            m_handDrawnNewEllipse = false;
        }
        qreal radius =  qAbs(event->scenePos().x() - m_handDrawnEllipseOrigine.x());
        qreal w      =  radius * 2;
        qreal h      = w;
        qreal x = m_handDrawnEllipseOrigine.x() - radius;
        qreal y = m_handDrawnEllipseOrigine.y() - radius;

        m_handDrawnEllipse->setRect(x, y, w, h);
        m_viewer->setCercleRadius(QPointF(w / m_tabSize.x(), h / m_tabSize.y()));
    }
    QGraphicsScene::mouseMoveEvent(event);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_handDrawingLineOn && !m_handDrawnNewLine) {
        if (m_mousePressed) {
            qreal xfrom = event->buttonDownScenePos(event->button()).x() /m_tabSize.x();
            qreal yfrom = event->buttonDownScenePos(event->button()).y() /m_tabSize.y();
            qreal xto   = event->lastScenePos().x() /m_tabSize.x();
            qreal yto   = event->lastScenePos().y() /m_tabSize.y();
            m_msg.setText(QString("Sauvegarder ce tracé ? \n (%1, %2) -> (%3, %4)").arg(xfrom,4, 'g', 3).
                                                                                     arg(yfrom, 4,'g', 3).
                                                                                     arg(xto, 4, 'g', 3).
                                                                                     arg(yto, 4, 'g', 3));
            if(m_msg.exec() == QMessageBox::Yes){
                addHandDrawnLine(m_handDrawnLine);
            } else
                removeItem(m_handDrawnLine);
            m_viewer->setLigneEnd(QPointF(0, 0), false);
        }
    } else if(m_handDrawingEllipseOn && !m_handDrawnNewEllipse) {
        if (m_mousePressed) {
            qreal x   = (m_handDrawnEllipse->rect().x() + m_handDrawnEllipse->rect().width() / 2.) /m_tabSize.x();
            qreal y   = (m_handDrawnEllipse->rect().y() + m_handDrawnEllipse->rect().height() / 2.) /m_tabSize.y();
            qreal w   = (m_handDrawnEllipse->rect().width() / 2.) /m_tabSize.x();
            qreal h   = w;
            m_msg.setText(QString("Sauvegarder ce tracé ? \n Origine = (%1, %2) R = (%3, %4)").arg(x,4, 'g', 3).
                                                                                     arg(y, 4,'g', 3).
                                                                                     arg(w, 4, 'g', 3).
                                                                                     arg(h, 4, 'g', 3));
            if(m_msg.exec() == QMessageBox::Yes){
                addHandDrawnEllipse(m_handDrawnEllipse);
                m_handDrawnEllipse->updateSceneInfo(false);
            } else
                removeItem(m_handDrawnEllipse);

        }
    } else
        QGraphicsScene::mouseReleaseEvent(event);
    m_mousePressed = false;
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::setCrossHairCenter(QPointF center)
{
    if (!m_crossHairOn)
        return;
    qreal xbeg = 0.0, xend = 1.0 *m_tabSize.x();
    qreal ybeg = center.y(), yend = center.y();
    m_crossHairH->setLine(xbeg, ybeg, xend, yend);
    xbeg = xend = center.x();
    ybeg = 0.0;
    yend = 1.0 *m_tabSize.y();
    m_crossHairV->setLine(xbeg, ybeg, xend, yend);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::showAllHandDrawn()
{
    m_handDrawingEllipseOn = m_handDrawingLineOn = false;
    QList<QGraphicsEllipseItem*>::iterator ite;
    for (ite = m_handDrawnEllipses.begin(); ite != m_handDrawnEllipses.end(); ++ite)
        (*ite)->show();

    QList<QGraphicsLineItem*>::iterator itl;
    for (itl = m_handDrawnLines.begin(); itl != m_handDrawnLines.end(); ++itl)
        (*itl)->show();
}

///////////////////////////////////////////////////////////////////////////////////////////
RegulItemGroup *RegulScene::setCarreaux()
{
    m_carreaux = new RegulItemGroup;
    m_carreaux->setSubType(RegulItemGroup::kCarreaux);
    return m_carreaux;
}

///////////////////////////////////////////////////////////////////////////////////////////
RegulItemGroup *RegulScene::setDiagonales()
{
    m_diagonales = new RegulItemGroup;
    m_diagonales->setSubType(RegulItemGroup::kCarreaux);
    return m_diagonales;
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::removeEllipse(RegulEllipseItem *ellipse)
{
    removeItem(ellipse);
    for (int index = 0; index < m_handDrawnEllipses.size(); index++) {
        if (m_handDrawnEllipses.at(index)->rect() == ellipse->rect())
            m_handDrawnEllipses.removeAt(index);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::removeLigne(RegulLineItem *ligne)
{
    removeItem(ligne);
    for (int index = 0; index < m_handDrawnLines.size(); index++) {
        if (m_handDrawnLines.at(index)->line() == ligne->line())
            m_handDrawnLines.removeAt(index);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::showHandDrawnEllispe()
{
    QList<QGraphicsEllipseItem*>::iterator it;
    for (it = m_handDrawnEllipses.begin(); it != m_handDrawnEllipses.end(); ++it)
        (*it)->show();

    m_handDrawingEllipseOn = true;
    addCrossHair();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::showHandDrawnLine()
{
    QList<QGraphicsLineItem*>::iterator it;
    for (it = m_handDrawnLines.begin(); it != m_handDrawnLines.end(); ++it)
        (*it)->show();

    m_handDrawingLineOn = true;
    addCrossHair();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulScene::updateCarreau(const QPointF &carreaux)
{
    if(m_carreaux) {
        for (QGraphicsLineItem* line : m_carreaux->list())
            removeItem(line);
        delete m_carreaux;
    }
    m_carreaux = new RegulItemGroup();
    m_carreaux->makeCarreaux(m_tabSize, carreaux.x(), carreaux.y());
    for (QGraphicsLineItem* line : m_carreaux->list()){
        addItem(line);
        line->show();
    }
}

