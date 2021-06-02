#ifndef REGULIMAGER_H
#define REGULIMAGER_H

#include "regulviewer.h"

#include <QMainWindow>
#include <QImage>

#if defined(QT_PRINTSUPPORT_LIB)
#  include <QtPrintSupport/qtprintsupportglobal.h>

#  if QT_CONFIG(printer)
#    include <QPrinter>
#  endif
#endif

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QDoubleSpinBox;

QT_END_NAMESPACE


class QGroupBox;
class RegulToolBox;
class RegulImager : public QMainWindow
{
    Q_OBJECT

public:
    RegulImager(QWidget *parent = nullptr);

    void          assignToolBox();
    QPoint        carreaux() const                                   {return QPoint(m_carreauxW, m_carreauxH);}
    QPointF       getRealSize()                                      {return QPointF(m_originalWidth, m_originalHeight);}
    QImage        image() const                                      {return m_image;}
    QImage        imageNetB() const                                  {return m_imageNetB;}
    void          init();
    bool          isCarreauxOn() const                               {return m_carreauxOn;}
    bool          isToolBoxOn() const                                {return m_toolBoxOn;}
    void          loadImage(const QString &imageFileName);
    void          setCarreaux(const QPoint& car)                     {m_carreauxW = car.x(); m_carreauxH = car.y();}
    void          setImageSize(int w, int h);
    void          setRealSize(qreal w, qreal h);
    void          setTraceSaved(bool val)                            {m_traceSaved = val;}
    void          setUnits(const QPointF& u)                         {m_units = u;}
    RegulViewer*  theView() const                                    {return m_viewer;}
    RegulToolBox* theToolBox() const                                 {return m_toolBox;}

    void close();
    void view();
    void keyPressed(QKeyEvent* evt);

public slots:
    void toolBox();

private slots:
    void carreau();
    void crayon();
    void crossHair();
    void diagonales();
    void ellipse();
    void fitSize();
    void imageOffset();
    void noiretBlanc();
    void normalSize();
    void texIt();
    void updateCercleValues(bool show = true);
    void updateCursorPosValue();
    void updateCrossHairPos();
    void updateLigneValues(bool show = true);
    void zoomIn();
    void zoomOut();

private:
    void createActions();
    void createMenus();
    void miseAuCarreaux(const QSize &size);
    bool saveFile(const QString &fileName);
    void updateActions();

    int             m_carreauxH;
    int             m_carreauxW;
    QAction*        m_carreauxAct;
    QGroupBox*      m_cercleInfoBox;
    QDoubleSpinBox* m_cercleXValueBox;
    QDoubleSpinBox* m_cercleYValueBox;
    QDoubleSpinBox* m_cercleRValueBox;
    QGroupBox*      m_cursorInfoBox;
    QAction*        m_crayonAct;
    QAction*        m_crosshairAct;
    QAction*        m_diagoAct;
    QAction*        m_ellipseAct;
    QAction*        m_fitSizeAct;
    QImage          m_image;
    QImage          m_imageNetB;
    QAction*        m_imageOffsetAct;
    const qreal     m_kcm2pixel = 28.45276;
    QGroupBox*      m_ligneInfoBox;
    QDoubleSpinBox* m_ligneXbegValueBox;
    QDoubleSpinBox* m_ligneYbegValueBox;
    QDoubleSpinBox* m_ligneXendValueBox;
    QDoubleSpinBox* m_ligneYendValueBox;
    QAction*        m_noirEtBlancAct;
    QString         m_nomTableau;
    QString         m_nomPeintre;
    QAction*        m_normalSizeAct;
    qreal           m_originalHeight;
    qreal           m_originalWidth;
    QAction*        m_printAct;
    RegulViewer*    m_viewer;
    QAction*        m_saveAsAct;
    qreal           m_scaleW = 1.;
    qreal           m_scaleH = 1.;
    QScrollArea*    m_scrollArea;
    QAction*        m_texAct;
    RegulToolBox*   m_toolBox;
    QAction*        m_toolBoxAct;
    QToolBar*       m_tools;
    QPointF         m_units = QPointF(1.0, 1.0);
    QDoubleSpinBox* m_xValueBox;
    QDoubleSpinBox* m_yValueBox;
    QAction*        m_viewAct;
    QAction*        m_zoomInAct;
    QAction*        m_zoomOutAct;

    bool            m_carreauxOn    = false;
    bool            m_crayonOn      = false;
    bool            m_crossHairOn   = false;
    bool            m_diagoOn       = false;
    bool            m_ellipseOn     = false;
    bool            m_noirEtBlancOn = false;
    bool            m_toolBoxOn     = false;
    bool            m_traceSaved    = false;
    bool            m_viewOn        = false;
};

#endif // REGULIMAGER_H
