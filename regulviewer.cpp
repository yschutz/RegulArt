#include "regulimager.h"
#include "regulscene.h"
#include "regulviewer.h"


#include <QColorSpace>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QImageReader>
#include <QMouseEvent>

///////////////////////////////////////////////////////////////////////////////////////////
RegulViewer::RegulViewer(QWidget *parent) : QGraphicsView(parent)
{
    m_imager = dynamic_cast<RegulImager*>(parent);
    m_scene = new RegulScene(this);
    setScene(m_scene);
    setStyleSheet("background: transparent");
    setFrameStyle(0);

}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulViewer::activateCrossHair(bool val)
{
    if (val)
        m_scene->addCrossHair();
    else
        m_scene->hideCrossHair();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulViewer::dessineEllipse(bool val)
{
    if(val)
        m_scene->showHandDrawnEllispe();
    else
        m_scene->hideHandDrawnEllispe();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulViewer::dessineLigne(bool val)
{
    if (val)
        m_scene->showHandDrawnLine();
    else
        m_scene->hideHandDrawnLine();

}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulViewer::init()
{
    m_cercleRadius = QPointF(0.0, 0.0);
    m_cercleCenter.setX(0.0);
    m_cercleCenter.setY(0.0);

    m_ligneBeg.setX(0.0);
    m_ligneBeg.setY(0.0);
    m_ligneEnd.setX(0.0);
    m_ligneEnd.setY(0.0);

    m_mouseX = m_mouseY = 0.0;

    m_scene->init();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulViewer::loadImage(const QImage &image)
{
    const qreal kcm2pixel = 28.45276;
    qreal w = m_imager->getRealSize().x()  * kcm2pixel;
    qreal h = m_imager->getRealSize().y() * kcm2pixel;
    setFixedSize(w, h);
    m_scene->setTabSize(QPointF(w, h));
    setSceneRect(0, 0, w, h);
    m_scene->addBoundingBox(w, h);
    m_scene->addImage(image.scaled(w, h, Qt::KeepAspectRatio));
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulViewer::miseAuCarreau(bool val, int w, int h)
{
    if (val)
        m_scene->addCarreaux(w, h);
    else
        m_scene->hideCarreau();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulViewer::setCercleCenter(QPointF center, bool show)
{
    m_cercleCenter = center;
    emit cercleChanged(show);
}


///////////////////////////////////////////////////////////////////////////////////////////
void RegulViewer::setCercleRadius(const QPointF &r, bool show)
{
    m_cercleRadius = r;
    emit cercleChanged(show);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulViewer::setLigneBeg(const QPointF &beg, bool show)
{
    m_ligneBeg = beg;
    emit ligneChanged(show);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulViewer::setLigneEnd(const QPointF &end, bool show)
{
    m_ligneEnd = end;
    emit ligneChanged(show);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulViewer::setMousePosVal(qreal x, qreal y)
{
    m_mouseX = x;
    m_mouseY = y;
    emit mouseMoved();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulViewer::setNetB(bool val)
{
    if (val)
        m_scene->addImage(m_imager->imageNetB());
    else
        m_scene->addImage(m_imager->image());
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulViewer::traceDiagonales(bool val)
{
    if (val)
        m_scene->addDiagonales();
    else
        m_scene->hideDiagonales();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulViewer::showTraces(bool val)
{
    if(val)
        m_scene->showAllHandDrawn();
    else
        m_scene->hideAllHanDrawn();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulViewer::keyPressEvent(QKeyEvent *evt)
{
    m_imager->keyPressed(evt);
    QGraphicsView::keyPressEvent(evt);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulViewer::updateCrossHairPosition(QPointF pos)
{
    pos.setX(pos.x() * sceneRect().width());
    pos.setY(pos.y() * sceneRect().height());
    m_scene->setCrossHairCenter(pos);
}
