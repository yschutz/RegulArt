#ifndef REGULTOOLBOX_H
#define REGULTOOLBOX_H

#include <QObject>
#include <QPen>
#include <QWidget>

class QCheckBox;
class QComboBox;
class QDial;
class QDoubleSpinBox;
class QGraphicsItem;
class QMenu;
class QPushButton;
class QSpinBox;
class QTabWidget;
class RegulEllipseItem;
class RegulLineItem;
class RegulItemGroup;
class RegulScene;
class RegulToolBox : public QWidget
{
    Q_OBJECT
public:
    enum LineStyle {kSOLID, kDASH, kDOT, kDASHSOT, kDASHSOTSOT, kCUSTOM};
    enum UnitType {kWH, kCARREAU, kABS};
    explicit RegulToolBox(QWidget *parent = nullptr);
    ~RegulToolBox();

    void    ellipseDispositionPage();
    void    ellipseStylePage();
    void    gridPage();
    void    setEllipse(RegulEllipseItem *ellipse);
    void    lineDispositionPage();
    void    lineStylePage();
    void    setGrid(RegulScene *scene);
    void    setLine(RegulLineItem* line);
    QPointF units() const;

signals:

private slots:
    void rotateEllipse();
    void rotateLine();
    void setClip();
    void setEllipseUnits(UnitType index);
    void setLineStyle(int index);
    void setLineUnits(UnitType index);
    void updateEllipse();
    void updateGrid(RegulScene *scene);
    void updateLine();


private:
    QDoubleSpinBox* m_cellsNx;
    QDoubleSpinBox* m_cellsNy;
    QDoubleSpinBox* m_cellsTx;
    QDoubleSpinBox* m_cellsTy;
    QGraphicsItem*  m_currentItem;
    QPen            m_currentPen;
    QCheckBox*      m_ellipseClip;
    QWidget*        m_ellipseDispositionPage;
    QDoubleSpinBox* m_ellipseHeight;
    QCheckBox*      m_ellipseHW;
    qreal           m_ellipseProp;
    QDoubleSpinBox* m_ellipseRotS;
    QDial*          m_ellipseRotW;
    QWidget*        m_ellipseStylePage;
    QDoubleSpinBox* m_ellipseWidth;
    QDoubleSpinBox* m_ellipseX;
    QDoubleSpinBox* m_ellipseY;
    QWidget*        m_gridPage;
    QPushButton*    m_gridOK;
    QDoubleSpinBox* m_lineBegValX;
    QDoubleSpinBox* m_lineBegValY;
    QPushButton*    m_lineColor;
    QDoubleSpinBox* m_lineEndValX;
    QDoubleSpinBox* m_lineEndValY;
    QWidget*        m_lineDispositionPage;
    QDoubleSpinBox* m_lineRotS;
    QDial*          m_lineRotW;
    QComboBox*      m_lineStyleBox;
    QWidget*        m_lineStylePage;
    QSpinBox*       m_lineThickness;
    QTabWidget*     m_tabs;
    QPointF         m_units;
    UnitType        m_unitCurrent;
    QMenu*          m_unitsMenuE;
    QMenu*          m_unitsMenuL;
    QPushButton*    m_unitsButtonE;
    QPushButton*    m_unitsButtonL;
    QAction*        m_unitAbsoluteActE;
    QAction*        m_unitAbsoluteActL;
    QAction*        m_unitPerCentCarreauActE;
    QAction*        m_unitPerCentCarreauActL;
    QAction*        m_unitPerCentWHActE;
    QAction*        m_unitPerCentWHActL;
};

#endif // REGULTOOLBOX_H
