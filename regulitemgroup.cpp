#include "regulimager.h"
#include "regulitemgroup.h"
#include "regullineitem.h"
#include "regulscene.h"
#include "regultoolbox.h"
#include "regulviewer.h"

///////////////////////////////////////////////////////////////////////////////////////////
//RegulItemGroup::RegulItemGroup(QGraphicsItem *parent) : QGraphicsItemGroup(parent)
  RegulItemGroup::RegulItemGroup(QObject *parent) : QObject(parent)
{
    m_pen.setStyle(Qt::SolidLine);
    m_pen.setWidth(3);
    m_pen.setBrush(Qt::white);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulItemGroup::makeCarreaux(const QPointF &tabSize, int nw, int nh)
{
    qreal xstep = 1.0 / nw;
    qreal ystep = 1.0 / nh;
    qreal xbeg  = 0.0, xend = 0.0;
    qreal ybeg  = 0.0, yend = tabSize.y();

    for (int index = 0; index <= nw; ++index) {
        xbeg = xend = index * xstep * tabSize.x();
        RegulLineItem * hori = new RegulLineItem(xbeg, ybeg, xend, yend, m_pen);
        hori->setIngroup();
//        addToGroup(hori);
        hori->setType(RegulLineItem::TypeC);
        addToList(hori);
    }
    xbeg = 0;
    xend = 1.0 * tabSize.x();
    for (int index = 0; index <= nh; ++index) {
        ybeg = yend = index * ystep * tabSize.y();
        RegulLineItem * verti = new RegulLineItem(xbeg, ybeg, xend, yend, m_pen);
        verti->setIngroup();
//        addToGroup(verti);
        verti->setType(RegulLineItem::TypeC);
        addToList(verti);
    }
//    setSubType(kCarreaux);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulItemGroup::makeDiagonales(const QPointF &tabSize)
{
    qreal xbeg = 0.0, xend = tabSize.x();
    qreal ybeg = 0.0, yend = tabSize.y();
    RegulLineItem * dia = new RegulLineItem(xbeg, ybeg, xend, yend, m_pen);
    dia->setIngroup();
//    addToGroup(dia);
    dia->setType(RegulLineItem::TypeD);
    addToList(dia);

    xend = tabSize.x();
    yend = tabSize.y() / 2.0;
    dia = new RegulLineItem(xbeg, ybeg, xend, yend, m_pen);
    dia->setIngroup();
//    addToGroup(dia);
    dia->setType(RegulLineItem::TypeD);
    addToList(dia);

    xend = tabSize.x() / 2.0;
    yend = tabSize.y();
    dia = new RegulLineItem(xbeg, ybeg, xend, yend, m_pen);
    dia->setIngroup();
//    addToGroup(dia);
    dia->setType(RegulLineItem::TypeD);
    addToList(dia);

    xbeg = tabSize.x(); xend = 0;
    dia = new RegulLineItem(xbeg, ybeg, xend, yend, m_pen);
    dia->setIngroup();
//    addToGroup(dia);
    dia->setType(RegulLineItem::TypeD);
    addToList(dia);

    xend = tabSize.x() / 2.0;
    yend = tabSize.y();
    dia = new RegulLineItem(xbeg, ybeg, xend, yend, m_pen);
    dia->setIngroup();
//    addToGroup(dia);
    dia->setType(RegulLineItem::TypeD);
    addToList(dia);

    xend = 0;
    yend = tabSize.y() / 2.0;
    dia = new RegulLineItem(xbeg, ybeg, xend, yend, m_pen);
    dia->setIngroup();
//    addToGroup(dia);
    dia->setType(RegulLineItem::TypeD);
    addToList(dia);

    xbeg = tabSize.x(); xend = tabSize.x() / 2.0;
    ybeg = tabSize.y(); yend = 0.0;
    dia = new RegulLineItem(xbeg, ybeg, xend, yend, m_pen);
    dia->setIngroup();
//    addToGroup(dia);
    dia->setType(RegulLineItem::TypeD);
    addToList(dia);

    xend = 0;
    yend = tabSize.y() / 2.0;
    dia = new RegulLineItem(xbeg, ybeg, xend, yend, m_pen);
    dia->setIngroup();
//    addToGroup(dia);
    dia->setType(RegulLineItem::TypeD);
    addToList(dia);

    xbeg = 0; xend = tabSize.x() / 2.0;
    ybeg = tabSize.y(); yend = 0.0;
    dia = new RegulLineItem(xbeg, ybeg, xend, yend, m_pen);
    dia->setIngroup();
//    addToGroup(dia);
    dia->setType(RegulLineItem::TypeD);
    addToList(dia);

    xend = tabSize.x();
    yend = tabSize.y() / 2.0;
    dia = new RegulLineItem(xbeg, ybeg, xend, yend, m_pen);
    dia->setIngroup();
//    addToGroup(dia);
    dia->setType(RegulLineItem::TypeD);
    addToList(dia);

    xbeg = 0; xend = tabSize.x();
    ybeg = yend = tabSize.y() / 2.0;
    dia = new RegulLineItem(xbeg, ybeg, xend, yend, m_pen);
    dia->setIngroup();
//    addToGroup(dia);
    dia->setType(RegulLineItem::TypeD);
    addToList(dia);

    xbeg = xend = tabSize.x() / 2.0;
    ybeg = 0; yend = tabSize.y();
    dia = new RegulLineItem(xbeg, ybeg, xend, yend, m_pen);
    dia->setIngroup();
//    addToGroup(dia);
    dia->setType(RegulLineItem::TypeD);
    addToList(dia);

//    setSubType(kDiagonales);
}

///////////////////////////////////////////////////////////////////////////////////////////
//void RegulItemGroup::mousePressEvent(QGraphicsSceneMouseEvent */*evt*/)
//{
//    RegulScene* theScene = dynamic_cast<RegulScene*>(scene());
//    RegulViewer * viewer = theScene->viewer();
//    RegulImager * imager = viewer->imager();
//    if (subType() == kCarreaux) {
//        if (imager->isToolBoxOn())
//            imager->theToolBox()->setGrid(theScene);
//    }
//     QGraphicsItemGroup::mousePressEvent(evt);
//}
