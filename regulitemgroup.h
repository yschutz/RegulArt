#ifndef REGULITEMGROUP_H
#define REGULITEMGROUP_H

#include <QGraphicsItemGroup>
#include <QPen>


class RegulItemGroup : public QObject//, public QGraphicsItemGroup
{
    Q_OBJECT
public:
//    enum {Type = UserType + 3};
//    enum SUBTYPE {kCarreaux, kDiagonales};

//    RegulItemGroup(QGraphicsItem *parent = nullptr);
    RegulItemGroup(QObject *parent = nullptr);

    bool    isMovable() const                            {return m_movable;}
    void    makeCarreaux(const QPointF &tabSize, int nw, int nh);
    void    makeDiagonales(const QPointF &tabSize);
//    void    mousePressEvent(QGraphicsSceneMouseEvent* evt) override;
    void    setMovable(bool val)                         {m_movable = val;}
//    void    setSubType(SUBTYPE val)                      {m_subType = val;}
//    int     type() const override                        {return Type;}
//    SUBTYPE subType() const                              {return m_subType;}

    void                      addToList(QGraphicsLineItem* line) {m_lines.append(line);}
    QList<QGraphicsLineItem*> list()                             {return m_lines;}

private:
    bool                      m_movable;
    QPen                      m_pen;
//    SUBTYPE                   m_subType;
    QList<QGraphicsLineItem*> m_lines;
};

#endif // REGULITEMGROUP_H
