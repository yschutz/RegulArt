#ifndef REGULELLIPSEITEM_H
#define REGULELLIPSEITEM_H

#include <QGraphicsEllipseItem>
#include <QMessageBox>
#include <QPen>

class RegulEllipseItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    enum {Type = UserType + 4};

    RegulEllipseItem(QGraphicsItem *parent = nullptr );
    RegulEllipseItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr);

    QPainterPath   boundaryPath() const            {return m_boundaryPath;}
    QPointF        center() const                  {return m_center;}
    QPen           currentPen() const              {return m_currentPen;}
    QList<QPointF> intersects() ;
    bool           isHandDrawn() const             {return m_handDrawn;}
    bool           isInsideTab() const;
    void           paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPointF        radius() const                  {return m_radius;}
    void           setBoundaryPath(bool val=true);
    void           setClip(bool test);
    void           setCenter(const QPointF &p)     {m_center = p;}
    void           setCurrentPen(const QPen &pen)  {m_currentPen = pen;}
    void           setHandDrawn(bool val);
    void           setRadius(QPointF r)              {m_radius = r;}
    int            type() const override           {return Type;}
    void           updateSceneInfo(bool show = true);

public slots:

protected:
    void     hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void     hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void     keyPressEvent(QKeyEvent *event) override;
    void     mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void     mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void     mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void init();
    void solve(qreal a, qreal b, qreal c, qreal &res1, qreal& res2);

    QPointF      m_center;
    QPen         m_currentPen;
    QPen         m_cyanPen;
    QPen         m_grayPen;
    QMessageBox  m_msg;
    QPainterPath m_boundaryPath;
    QPointF      m_radius;
    QPen         m_redPen;
    qreal        m_scale     = 1.0;
    bool         m_handDrawn = false;
};

#endif // REGULELLIPSEITEM_H
