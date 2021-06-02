#ifndef REGULLINEITEM_H
#define REGULLINEITEM_H


#include <QDataStream>
#include <QGraphicsLineItem>
#include <QMessageBox>
#include <QObject>
#include <QPen>

class RegulLineItem : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    enum {Type = UserType + 1, TypeC, TypeD};

    RegulLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = nullptr);
    RegulLineItem(qreal x1, qreal y1, qreal x2, qreal y2, const QPen &pen, QGraphicsItem *parent = nullptr);
    RegulLineItem(QGraphicsItem *parent = nullptr);

    virtual ~RegulLineItem() = default;

    QPointF equation() const;
    bool    isHandDrawn() const           {return m_handDrawn;}
    bool    isInGroup() const             {return m_ingroup;}
    void    setHandDrawn(bool val);
    void    setIngroup(bool val = true)   {m_ingroup = val;}
    void    setMyLine(qreal x1, qreal y1, qreal x2, qreal y2);
    void    setType(int type = Type)      {m_type = type;}
    int     type() const override {return Type;}

signals:
    void    lineSelected();

protected:
    void     hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void     hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void     keyPressEvent(QKeyEvent *event) override;
    void     mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void     mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void init();
    void updateSceneInfo(bool show = true);

    QPointF     m_beg;
    QPen        m_currentPen;
    QPen        m_cyanPen;
    QPointF     m_end;
    QPen        m_grayPen;
    QPointF     m_mend;
    QMessageBox m_msg;
    QPen        m_redPen;
    qreal       m_rotate     = 0.0;
    qreal       m_scale      = 1.1;
    int         m_type       = Type;
    bool        m_handDrawn  = false;
    bool        m_ingroup    = false;
};

#endif // REGULLINEITEM_H
