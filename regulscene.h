#ifndef REGULSCENE_H
#define REGULSCENE_H

#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QMessageBox>

class RegulItemGroup;
class RegulEllipseItem;
class RegulLineItem;
class RegulViewer;
class RegulScene : public QGraphicsScene
{
    Q_OBJECT

public:
    RegulScene(QObject *parent = nullptr);

    void            addBoundingBox(qreal w, qreal h);
    void            addCarreaux(int nw, int nh);
    void            addCrossHair();
    void            addDiagonales();
    void            addHandDrawnEllipse(RegulEllipseItem* item);
    void            addHandDrawnLine(RegulLineItem *ligne);
    void            addImage(const QImage &image);
    void            addMyText(const QString &text, QPointF pos);
    void            hideAllHanDrawn();
    void            hideCarreau();
    void            hideCrossHair();
    void            hideDiagonales();
    void            hideHandDrawnEllispe();
    void            hideHandDrawnLine();
    QPointF         ImageOffset() const                   {return m_image->offset();}
    void            init();
    void            removeEllipse(RegulEllipseItem *ellipse);
    void            removeLigne(RegulLineItem *ligne);
    void            setCrossHairCenter(QPointF center);
    void            setTabSize(const QPointF &tabSize)    {m_tabSize = tabSize;}
    void            showAllHandDrawn();
    RegulViewer*    viewer() const                        {return m_viewer;}
    RegulItemGroup* setCarreaux();
    RegulItemGroup* setDiagonales();
    void            setImageOffset(const QPointF &offset) {m_image->setOffset(offset);}
    void            showHandDrawnEllispe();
    void            showHandDrawnLine();
    QPointF         tabSize() const                       {return m_tabSize;}
    void            updateCarreau(const QPointF& carreaux);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void mouseMoved();

private:
    RegulItemGroup*              m_carreaux;
    QGraphicsLineItem*           m_crossHairH;
    QGraphicsLineItem*           m_crossHairV;
    RegulItemGroup*              m_diagonales;
    RegulEllipseItem*            m_handDrawnEllipse;
    RegulLineItem*               m_handDrawnLine;
    QPointF                      m_handDrawnEllipseOrigine;
    QPointF                      m_handDrawnLineOrigine;
    QList<QGraphicsEllipseItem*> m_handDrawnEllipses;
    QList<QGraphicsLineItem*>    m_handDrawnLines;
    QGraphicsPixmapItem*         m_image;
    QMessageBox                  m_msg;
    QPointF                      m_tabSize;
    RegulViewer*                 m_viewer;
    bool                         m_crossHairOn          = false;
    bool                         m_handDrawingEllipseOn = false;
    bool                         m_handDrawingLineOn    = false;
    bool                         m_handDrawnNewEllipse  = true;
    bool                         m_handDrawnNewLine     = true;
    bool                         m_mousePressed         = false;
};

#endif // REGULSCENE_H
