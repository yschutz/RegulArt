#ifndef REGULVIEWER_H
#define REGULVIEWER_H

#include <QGraphicsView>

class RegulImager;
class RegulScene;
class RegulViewer : public QGraphicsView
{
    Q_OBJECT
public:
    RegulViewer(QWidget *parent = nullptr);
    ~RegulViewer() override {}

    void         activateCrossHair(bool val);
    QPointF      cercleRadius() const                     {return m_cercleRadius;}
    QPointF      cercleCenter() const                     {return m_cercleCenter;}
    void         dessineEllipse(bool val);
    void         dessineLigne(bool val);
    QSize        dimension() const                        {return maximumSize();}
    RegulImager* imager() const                           {return m_imager;}
    void         init();
    void         keyPressEvent(QKeyEvent* evt) override;
    QPointF      ligneBeg() const                         {return m_ligneBeg;}
    QPointF      ligneEnd() const                         {return m_ligneEnd;}
    void         loadImage(const QImage & image);
    void         miseAuCarreau(bool val, int w = 0, int h = 0);
    QPointF      mousePos() const                         {return QPointF(m_mouseX, m_mouseY);}
    RegulScene*  scene()                                  {return m_scene;}
    void         setCercleRadius(const QPointF &r, bool show = true);
    void         setCercleCenter(QPointF center, bool show = true);
    void         setLigneBeg(const QPointF &beg, bool show = true);
    void         setLigneEnd(const QPointF &end, bool show = true);
    void         setMousePosVal(qreal x, qreal y);
    void         setNetB(bool val);
    void         traceDiagonales(bool val);
    void         updateCrossHairPosition(QPointF pos);
    void         showTraces(bool val);

signals:
    void cercleChanged(bool show);
    void mouseMoved();
    void ligneChanged(bool show);

public slots:

private:
    QPointF        m_cercleRadius;
    QPointF        m_cercleCenter;
    QImage         m_imageNetB;
    RegulImager*   m_imager;
    QGraphicsItem* m_item;
    QPointF        m_ligneBeg;
    QPointF        m_ligneEnd;
    qreal          m_mouseX;
    qreal          m_mouseY;
    RegulScene*    m_scene;
};

#endif // REGULVIEWER_H
