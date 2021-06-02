#include "regulellipseitem.h"
#include "regulimager.h"
#include "regulitemgroup.h"
#include "regullineitem.h"
#include "regulscene.h"
#include "regultoolbox.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QDial>
#include <QDoubleSpinBox>
#include <QGuiApplication>
#include <QLabel>
#include <QMenu>
#include <QPen>
#include <QPushButton>
#include <QScreen>
#include <QStackedWidget>
#include <QTabBar>
#include <QTabWidget>

///////////////////////////////////////////////////////////////////////////////////////////
RegulToolBox::RegulToolBox(QWidget *parent) : QWidget(parent),
    m_currentItem(nullptr),
    m_ellipseDispositionPage(nullptr), m_ellipseHeight(nullptr), m_ellipseStylePage(nullptr), m_ellipseWidth(nullptr),
    m_ellipseX(nullptr), m_ellipseY(nullptr), m_gridPage(nullptr),
    m_lineBegValX(nullptr), m_lineBegValY(nullptr), m_lineDispositionPage(nullptr), m_lineStylePage(nullptr),
    m_unitCurrent(kWH)

{
    m_units.setX(1.0);
    m_units.setY(1.0);

    m_tabs = new QTabWidget(this);
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addWidget(m_tabs);
    setLayout(mainlayout);

    setWindowTitle(tr("Boite à outils"));
    setWindowFlag(Qt::Window, true);
    sizeHint();
}

///////////////////////////////////////////////////////////////////////////////////////////
RegulToolBox::~RegulToolBox()
{
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::ellipseDispositionPage()
{

    if (m_ellipseDispositionPage)
        return;

    m_ellipseDispositionPage = new QWidget;

    // ligne horizontale Top
    QFrame* lineTop1 = new QFrame(m_ellipseDispositionPage);
    lineTop1->setFrameShape(QFrame::HLine);
    lineTop1->setFrameShadow(QFrame::Sunken);

    // label Taille
    QLabel* taillelab = new QLabel(tr("Taille"), m_ellipseDispositionPage);

    // sélecteur hauteur
    m_ellipseHeight = new QDoubleSpinBox(m_ellipseDispositionPage);
    m_ellipseHeight->setRange(0.0, 300);
    m_ellipseHeight->setDecimals(3);
    m_ellipseHeight->setSingleStep(0.001);
    m_ellipseHeight->setObjectName("H");

    // label Hauteur
    QLabel* hlabel = new QLabel(tr("Hauteur"));

    // sélecteur + label hauteur
    QVBoxLayout* lyVBH = new QVBoxLayout;
    lyVBH->addWidget(m_ellipseHeight);
    lyVBH->addWidget(hlabel);
    lyVBH->setAlignment(hlabel, Qt::AlignHCenter);

    // sélecteur largeur
    m_ellipseWidth = new QDoubleSpinBox(m_ellipseDispositionPage);
    m_ellipseWidth->setRange(0.0, 300);
    m_ellipseWidth->setDecimals(3);
    m_ellipseWidth->setSingleStep(0.001);
    m_ellipseWidth->setObjectName("W");

    // label largeur
    QLabel* wlabel = new QLabel(tr("Largeur"));

    // sélecteur + label largeur
    QVBoxLayout* lyVBW = new QVBoxLayout;
    lyVBW->addWidget(m_ellipseWidth);
    lyVBW->addWidget(wlabel);
    lyVBW->setAlignment(wlabel, Qt::AlignHCenter);

    // sélecteurs + labels hauteur + largeur
    QHBoxLayout* lyHBHW = new QHBoxLayout;
    lyHBHW->addLayout(lyVBH);
    lyHBHW->addLayout(lyVBW);

    // option proportions
    m_ellipseHW = new QCheckBox(m_ellipseDispositionPage);

    //label proportions
    QLabel* rappProp = new QLabel(tr("Conserver les proportions"));

    // option + label proportion
    QHBoxLayout* lyHBRP = new QHBoxLayout;
    lyHBRP->addWidget(m_ellipseHW);
    lyHBRP->addWidget(rappProp);

    // option clip
    m_ellipseClip = new QCheckBox(m_ellipseDispositionPage);

    //label clip
    QLabel* clipLab = new QLabel(tr("Clip"));

    // option + label Clip
    QHBoxLayout* lyHBCL = new QHBoxLayout;
    lyHBCL->addWidget(m_ellipseClip);
    lyHBCL->addWidget(clipLab,0, Qt::AlignLeft);

    // sélecteurs + labels hauteur + largeur + option + label proportion + option + label clip
    QVBoxLayout* lyVBHW = new QVBoxLayout;
    lyVBHW->addLayout(lyHBHW);
    lyVBHW->addLayout(lyHBRP);
    lyVBHW->addLayout(lyHBCL);

    // label Taille + sélecteurs + labels hauteur + largeur + option + label proportion
    QHBoxLayout *lyHBT = new QHBoxLayout;
    lyHBT->addWidget(taillelab);
    lyHBT->setAlignment(taillelab, Qt::AlignTop);
    lyHBT->addLayout(lyVBHW);

//=========================================================
    QFrame* lineTop2 = new QFrame(m_ellipseDispositionPage);
    lineTop2->setFrameShape(QFrame::HLine);
    lineTop2->setFrameShadow(QFrame::Sunken);

    // label Position
    QLabel* positionlab = new QLabel(tr("Position"), m_ellipseDispositionPage);

    // sélecteur X
    m_ellipseX = new QDoubleSpinBox(m_ellipseDispositionPage);
    m_ellipseX->setRange(0.0, 300);
    m_ellipseX->setDecimals(3);
    m_ellipseX->setSingleStep(0.001);

    // label X
    QLabel* xlabel = new QLabel("X");

    // sélecteur + label hauteur
    QVBoxLayout* lyVBX = new QVBoxLayout;
    lyVBX->addWidget(m_ellipseX);
    lyVBX->addWidget(xlabel);
    lyVBX->setAlignment(xlabel, Qt::AlignHCenter);

    // sélecteur Y
    m_ellipseY = new QDoubleSpinBox(m_ellipseDispositionPage);
    m_ellipseY->setRange(0.0, 300);
    m_ellipseY->setDecimals(3);
    m_ellipseY->setSingleStep(0.001);

    // label Y
    QLabel* ylabel = new QLabel("Y");

    // sélecteur + label largeur
    QVBoxLayout* lyVBY = new QVBoxLayout;
    lyVBY->addWidget(m_ellipseY);
    lyVBY->addWidget(ylabel);
    lyVBY->setAlignment(ylabel, Qt::AlignHCenter);

    // sélecteurs + labels X + Y
    QHBoxLayout* lyHBXY = new QHBoxLayout;
    lyHBXY->addLayout(lyVBX);
    lyHBXY->addLayout(lyVBY);

    // label Position + sélecteurs + labels X + Y
    QHBoxLayout* lyHBP = new QHBoxLayout;
    lyHBP->addWidget(positionlab);
    lyHBP->setAlignment(positionlab, Qt::AlignTop);
    lyHBP->addLayout(lyHBXY);

    // box taille + box pos
    QVBoxLayout* lyVBTP = new QVBoxLayout;
    lyVBTP->addLayout(lyHBT);
    lyVBTP->addWidget(lineTop2);
    lyVBTP->addLayout(lyHBP);

    // unit sélection
    m_unitsMenuE = new QMenu(m_ellipseDispositionPage);
    m_unitPerCentWHActE = new QAction(tr("% H/L"));
    m_unitPerCentWHActE->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    m_unitsMenuE->setDefaultAction(m_unitPerCentWHActE);
    connect(m_unitPerCentWHActE, &QAction::triggered, [=](){RegulToolBox::setEllipseUnits(kWH);});
    m_unitsMenuE->addAction(m_unitPerCentWHActE);
    m_unitPerCentCarreauActE = new QAction(tr("% Carreau"));
    m_unitPerCentCarreauActE->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
    connect(m_unitPerCentCarreauActE, &QAction::triggered, [=](){RegulToolBox::setEllipseUnits(kCARREAU);});
    m_unitsMenuE->addAction(m_unitPerCentCarreauActE);
    m_unitAbsoluteActE = new QAction(tr("cm"));
    m_unitAbsoluteActE->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
    connect(m_unitAbsoluteActE, &QAction::triggered, [=](){RegulToolBox::setEllipseUnits(kABS);});
    m_unitsMenuE->addAction(m_unitAbsoluteActE);
    m_unitsButtonE = new QPushButton(m_lineDispositionPage);
    m_unitsButtonE->setText(m_unitsMenuE->defaultAction()->text());
    m_unitsButtonE->setMenu(m_unitsMenuE);
    setEllipseUnits(m_unitCurrent);

    // box taille + box pos + unités
    QHBoxLayout* lyHBTPU = new QHBoxLayout();
    lyHBTPU->addLayout(lyVBTP);
    lyHBTPU->addWidget(m_unitsButtonE);


//=========================================================
    QFrame* lineTop3 = new QFrame(m_ellipseDispositionPage);
    lineTop3->setFrameShape(QFrame::HLine);
    lineTop3->setFrameShadow(QFrame::Sunken);

    // label Rotation
    QLabel* rotationLab = new QLabel(tr("Rotation"), m_ellipseDispositionPage);

    // roue
    m_ellipseRotW = new QDial(m_ellipseDispositionPage);
    m_ellipseRotW->setRange(0., 360.);
    m_ellipseRotW->setSingleStep(1.);

    // sélecteur
    m_ellipseRotS = new QDoubleSpinBox(m_ellipseDispositionPage);
    m_ellipseRotS->setRange(0., 360.);
    m_ellipseRotS->setSingleStep(1.);
    m_ellipseRotS->setDecimals(0);

    // label Angle
    QLabel* angleLab = new QLabel(tr("Angle"), m_ellipseDispositionPage);

    // sélecteur + label Angle
    QVBoxLayout* lyVBA = new QVBoxLayout;
    lyVBA->addWidget(m_ellipseRotS);
    lyVBA->addWidget(angleLab);
    lyVBA->setAlignment(angleLab, Qt::AlignHCenter);

    // label Rotation + roue + sélecteur + label Angle
    QHBoxLayout* lyHBR = new QHBoxLayout;
    lyHBR->addWidget(rotationLab);
    lyHBR->addWidget(m_ellipseRotW);
    lyHBR->addLayout(lyVBA);
    lyHBR->setAlignment(rotationLab, Qt::AlignTop);
    lyHBR->setAlignment(m_ellipseRotW, Qt::AlignVCenter);

    QFrame* lineTop4 = new QFrame(m_ellipseDispositionPage);
    lineTop4->setFrameShape(QFrame::HLine);
    lineTop4->setFrameShadow(QFrame::Sunken);

    QVBoxLayout* lyPos = new QVBoxLayout;
    lyPos->addWidget(lineTop1);
    lyPos->addLayout(lyHBTPU);
    lyPos->addWidget(lineTop3);
    lyPos->addLayout(lyHBR);
    lyPos->addWidget(lineTop4);

    m_ellipseDispositionPage->setLayout(lyPos);    
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::ellipseStylePage()
{
    if (m_ellipseStylePage)
        return;
    m_ellipseStylePage = new QWidget;
    QHBoxLayout *lyH = new QHBoxLayout;
    QLabel *begLaby = new QLabel(tr("Début"));
    QLabel *endLaby = new QLabel(tr("Fin"));
    lyH->addWidget(begLaby);
    lyH->addWidget(endLaby);
    m_ellipseStylePage->setLayout(lyH);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::gridPage()
{
    if (m_gridPage)
        return;
    m_gridPage = new QWidget;

//=========================================================
    QFrame* lineTop1 = new QFrame(m_gridPage);
    lineTop1->setFrameShape(QFrame::HLine);
    lineTop1->setFrameShadow(QFrame::Sunken);

    // label cellules
    QLabel* cellLab = new QLabel(tr("Céllules"), m_gridPage);

    // sélecteur nbr cellules largeur
    m_cellsNx = new QDoubleSpinBox(m_gridPage);
    m_cellsNx->setRange(1, 30);
    m_cellsNx->setDecimals(0);
    m_cellsNx->setSingleStep(1);

    // label nx
    QLabel* nxLab = new QLabel("nX");

    // sélecteur + label x
    QVBoxLayout *lyVBNX = new QVBoxLayout;
    lyVBNX->addWidget(m_cellsNx);
    lyVBNX->addWidget(nxLab);
    lyVBNX->setAlignment(nxLab, Qt::AlignHCenter);

    // sélecteur nbr cellules hauteur
    m_cellsNy = new QDoubleSpinBox(m_gridPage);
    m_cellsNy->setRange(1, 30);
    m_cellsNy->setDecimals(0);
    m_cellsNy->setSingleStep(1);

    // label nx
    QLabel* nyLab = new QLabel("nY");

    // sélecteur + label y
    QVBoxLayout *lyVBNY = new QVBoxLayout;
    lyVBNY->addWidget(m_cellsNy);
    lyVBNY->addWidget(nyLab);
    lyVBNY->setAlignment(nyLab, Qt::AlignHCenter);

    // label cellules + sélecteurs + labels X + Y
    QHBoxLayout* lyHBN = new QHBoxLayout;
    lyHBN->addWidget(cellLab);
    lyHBN->addLayout(lyVBNX);
    lyHBN->addLayout(lyVBNY);
    lyHBN->setAlignment(cellLab, Qt::AlignTop);

    // label taille
    QLabel* sizeLab = new QLabel(tr("Taille"), m_gridPage);

    // sélecteur nbr cellules largeur
    m_cellsTx = new QDoubleSpinBox(m_gridPage);
    m_cellsTx->setRange(0.1, 100);
    m_cellsTx->setDecimals(2);
    m_cellsTx->setSingleStep(0.01);

    // label tx
    QLabel* txLab = new QLabel("l-cm");

    // sélecteur + label x
    QVBoxLayout *lyVBTX = new QVBoxLayout;
    lyVBTX->addWidget(m_cellsTx);
    lyVBTX->addWidget(txLab);
    lyVBTX->setAlignment(txLab, Qt::AlignHCenter);

    // sélecteur nbr cellules hauteur
    m_cellsTy = new QDoubleSpinBox(m_gridPage);
    m_cellsTy->setRange(0.1, 100);
    m_cellsTy->setDecimals(2);
    m_cellsTy->setSingleStep(0.01);

    // label tx
    QLabel* tyLab = new QLabel("h-cm");

    // sélecteur + label y
    QVBoxLayout *lyVBTY = new QVBoxLayout;
    lyVBTY->addWidget(m_cellsTy);
    lyVBTY->addWidget(tyLab);
    lyVBTY->setAlignment(tyLab, Qt::AlignHCenter);

    // label cellules + sélecteurs + labels X + Y
    QHBoxLayout* lyHBT = new QHBoxLayout;
    lyHBT->addWidget(sizeLab);
    lyHBT->addLayout(lyVBTX);
    lyHBT->addLayout(lyVBTY);
    lyHBT->setAlignment(sizeLab, Qt::AlignTop);

    QFrame* lineBot1 = new QFrame(m_gridPage);
    lineBot1->setFrameShape(QFrame::HLine);
    lineBot1->setFrameShadow(QFrame::Sunken);

    // OK button

    m_gridOK = new QPushButton("OK", m_gridPage);

    QVBoxLayout* lyPos = new QVBoxLayout;
    lyPos->addWidget(lineTop1);
    lyPos->addLayout(lyHBN);
    lyPos->addLayout(lyHBT);
    lyPos->addWidget(lineBot1);
    lyPos->addWidget(m_gridOK);

    m_gridPage->setLayout(lyPos);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::lineDispositionPage()
{
    if (m_lineDispositionPage)
        return;
    m_lineDispositionPage = new QWidget;

//=========================================================
    QFrame* lineTop1 = new QFrame(m_ellipseDispositionPage);
    lineTop1->setFrameShape(QFrame::HLine);
    lineTop1->setFrameShadow(QFrame::Sunken);

    // label début
    QLabel* begLab = new QLabel(tr("Début"), m_lineDispositionPage);

    // sélecteur X
    m_lineBegValX = new QDoubleSpinBox(m_lineDispositionPage);
    m_lineBegValX->setRange(0.0, 300);
    m_lineBegValX->setDecimals(3);
    m_lineBegValX->setSingleStep(0.001);

    // label X
    QLabel* xLab = new QLabel("X");

    // sélecteur + label X
    QVBoxLayout *lyVBXD = new QVBoxLayout;
    lyVBXD->addWidget(m_lineBegValX);
    lyVBXD->addWidget(xLab);
    lyVBXD->setAlignment(xLab, Qt::AlignHCenter);

    // sélecteur Y
    m_lineBegValY = new QDoubleSpinBox(m_lineDispositionPage);
    m_lineBegValY->setRange(0.0, 300);
    m_lineBegValY->setDecimals(3);
    m_lineBegValY->setSingleStep(0.001);

   // label Y
    QLabel* yLab = new QLabel("Y");

    // sélecteur + label Y
    QVBoxLayout* lyVBYD = new QVBoxLayout;
    lyVBYD->addWidget(m_lineBegValY);
    lyVBYD->addWidget(yLab);
    lyVBYD->setAlignment(yLab, Qt::AlignHCenter);

    // label debut + sélecteurs + labels X + Y
    QHBoxLayout* lyHBD = new QHBoxLayout;
    lyHBD->addWidget(begLab);
    lyHBD->addLayout(lyVBXD);
    lyHBD->addLayout(lyVBYD);
    lyHBD->setAlignment(begLab, Qt::AlignTop);


 //=========================================================

    // label fin
    QLabel* endLab = new QLabel(tr("Fin"), m_lineDispositionPage);

    // sélecteur X
    m_lineEndValX = new QDoubleSpinBox(m_lineDispositionPage);
    m_lineEndValX->setRange(0.0, 300);
    m_lineEndValX->setDecimals(3);
    m_lineEndValX->setSingleStep(0.001);

    // label X
    xLab = new QLabel("X");

    // sélecteur + label X
    QVBoxLayout* lyVBXF = new QVBoxLayout;
    lyVBXF->addWidget(m_lineEndValX);
    lyVBXF->addWidget(xLab);
    lyVBXF->setAlignment(xLab, Qt::AlignHCenter);

    // sélecteur Y
    m_lineEndValY = new QDoubleSpinBox(m_lineDispositionPage);
    m_lineEndValY->setRange(0.0, 300);
    m_lineEndValY->setDecimals(3);
    m_lineEndValY->setSingleStep(0.001);

    // label Y
    yLab = new QLabel("Y");

    // sélecteur + label Y
    QVBoxLayout* lyVBYF = new QVBoxLayout;
    lyVBYF->addWidget(m_lineEndValY);
    lyVBYF->addWidget(yLab);
    lyVBYF->setAlignment(yLab, Qt::AlignHCenter);

    //label fin + sélecteurs + labels X + Y
    QHBoxLayout* lyHBF = new QHBoxLayout;
    lyHBF->addWidget(endLab);
    lyHBF->addLayout(lyVBXF);
    lyHBF->addLayout(lyVBYF);
    lyHBF->setAlignment(endLab, Qt::AlignTop);

    // (label début) + sélecteurs + labels X + Y) + (label fin + sélecteurs + labels X + Y)
    QVBoxLayout* lyVBDF = new QVBoxLayout;
    lyVBDF->addLayout((lyHBD));
    lyVBDF->addLayout((lyHBF));

    // unit sélection
    m_unitsMenuL = new QMenu(m_lineDispositionPage);
    m_unitPerCentWHActL = new QAction(tr("% H/L"));
    m_unitPerCentWHActL->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    m_unitsMenuL->setDefaultAction(m_unitPerCentWHActL);
    connect(m_unitPerCentWHActL, &QAction::triggered, [=](){RegulToolBox::setLineUnits(kWH);});
    m_unitsMenuL->addAction(m_unitPerCentWHActL);
    m_unitPerCentCarreauActL = new QAction(tr("% Carreau"));
    m_unitPerCentCarreauActL->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
    connect(m_unitPerCentCarreauActL, &QAction::triggered, [=](){RegulToolBox::setLineUnits(kCARREAU);});
    m_unitsMenuL->addAction(m_unitPerCentCarreauActL);
    m_unitAbsoluteActL = new QAction(tr("cm"));
    m_unitAbsoluteActL->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
    connect(m_unitAbsoluteActL, &QAction::triggered, [=](){RegulToolBox::setLineUnits(kABS);});
    m_unitsMenuL->addAction(m_unitAbsoluteActL);
    m_unitsButtonL = new QPushButton(m_lineDispositionPage);
    m_unitsButtonL->setText(m_unitsMenuL->defaultAction()->text());
    m_unitsButtonL->setMenu(m_unitsMenuL);
    setLineUnits(m_unitCurrent);

    // (label début) + sélecteurs + labels X + Y) + (label fin + sélecteurs + labels X + Y) + unités
    QHBoxLayout* lyHBDFU = new QHBoxLayout;
    lyHBDFU->addLayout(lyVBDF);
    lyHBDFU->addWidget(m_unitsButtonL);

//=========================================================
    QFrame* lineTop2 = new QFrame(m_ellipseDispositionPage);
    lineTop2->setFrameShape(QFrame::HLine);
    lineTop2->setFrameShadow(QFrame::Sunken);

    // label Rotation
    QLabel* rotationLab = new QLabel(tr("Rotation"), m_lineDispositionPage);

    // roue
    m_lineRotW = new QDial(m_lineDispositionPage);
    m_lineRotW->setRange(0., 360.);
    m_lineRotW->setSingleStep(1.);

    // sélecteur
    m_lineRotS = new QDoubleSpinBox(m_lineDispositionPage);
    m_lineRotS->setRange(0., 360.);
    m_lineRotS->setSingleStep(1.);
    m_lineRotS->setDecimals(0);

    // label Angle
    QLabel* angleLab = new QLabel(tr("Angle"), m_lineDispositionPage);

    // sélecteur + label Angle
    QVBoxLayout* lyVBA = new QVBoxLayout;
    lyVBA->addWidget(m_lineRotS);
    lyVBA->addWidget(angleLab);
    lyVBA->setAlignment(angleLab, Qt::AlignHCenter);

    // label Rotation + roue + sélecteur + label Angle
    QHBoxLayout* lyHBR = new QHBoxLayout;
    lyHBR->addWidget(rotationLab);
    lyHBR->addWidget(m_lineRotW);
    lyHBR->addLayout(lyVBA);
    lyHBR->setAlignment(rotationLab, Qt::AlignTop);
    lyHBR->setAlignment(m_lineRotW, Qt::AlignVCenter);

    QFrame* lineTop3 = new QFrame(m_ellipseDispositionPage);
    lineTop3->setFrameShape(QFrame::HLine);
    lineTop3->setFrameShadow(QFrame::Sunken);

    QVBoxLayout* lyPos = new QVBoxLayout;
    lyPos->addWidget(lineTop1);
    lyPos->addLayout(lyHBDFU);
    lyPos->addWidget(lineTop2);
    lyPos->addLayout(lyHBR);
    lyPos->addWidget(lineTop3);

    m_lineDispositionPage->setLayout(lyPos);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::lineStylePage()
{
    if (m_lineStylePage)
        return;
    m_lineStylePage = new QWidget;

//=========================================================
    QFrame* lineTop1 = new QFrame(m_ellipseDispositionPage);
    lineTop1->setFrameShape(QFrame::HLine);
    lineTop1->setFrameShadow(QFrame::Sunken);

    // label Trait
    QLabel* traitlab = new QLabel(tr("trait"), m_lineStylePage);

    // style ligne
    m_lineStyleBox = new QComboBox(m_lineStylePage);
    m_lineStyleBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_lineStyleBox->setIconSize(QSize(32,32));
    m_lineStyleBox->addItem(QIcon(":/icons/qpen-solid.png"), "");
    m_lineStyleBox->addItem(QIcon(":/icons/qpen-dash.png"), "");
    m_lineStyleBox->addItem(QIcon(":/icons/qpen-dot.png"), "");
    m_lineStyleBox->addItem(QIcon(":/icons/qpen-dashdot.png"), "");
    m_lineStyleBox->addItem(QIcon(":/icons/qpen-dashdotdot.png"), "");
    m_lineStyleBox->addItem(QIcon(":/icons/qpen-custom.png"), "");

    // couleur ligne
    QIcon colIco(":/icons/rainbow.png");
    m_lineColor = new QPushButton(colIco, "", m_lineStylePage);

    // épaisseur ligne
    m_lineThickness = new QSpinBox(m_lineStylePage);
    m_lineThickness->setRange(1, 6);
    m_lineThickness->setSingleStep(1);
    m_lineThickness->setValue(3);

    QHBoxLayout* lyHBT = new QHBoxLayout;
    lyHBT->addWidget(m_lineStyleBox);
    lyHBT->addWidget(m_lineColor);
    lyHBT->addWidget(m_lineThickness);

    QVBoxLayout* lyVBT = new QVBoxLayout;
    lyVBT->addWidget(traitlab);
    lyVBT->setAlignment(traitlab, Qt::AlignLeft);
    lyVBT->addLayout(lyHBT);

    QVBoxLayout* lyPos = new QVBoxLayout;
    lyPos->addWidget(lineTop1);
    lyPos->addLayout(lyVBT);

    m_lineStylePage->setLayout(lyPos);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::setGrid(RegulScene* scene)
{
    if (m_lineDispositionPage && m_lineStylePage) {
        m_tabs->setTabVisible(m_tabs->indexOf(m_lineDispositionPage), false);
        m_tabs->setTabVisible(m_tabs->indexOf(m_lineStylePage), false);
    }
    if (m_ellipseDispositionPage && m_ellipseStylePage) {
        m_tabs->setTabVisible(m_tabs->indexOf(m_ellipseDispositionPage), false);
        m_tabs->setTabVisible(m_tabs->indexOf(m_ellipseStylePage), false);
    }

    if (m_gridPage)
        m_tabs->setTabVisible(m_tabs->indexOf(m_gridPage), false);
    else {
        gridPage();
        m_tabs->addTab(m_gridPage, tr("Mise au carreau"));
    }
    m_tabs->setTabVisible(m_tabs->indexOf(m_gridPage), true);


    RegulImager * imager = scene->viewer()->imager();
    m_cellsNx->setValue(imager->carreaux().x());
    m_cellsNy->setValue(imager->carreaux().y());

    m_cellsTx->setValue(imager->getRealSize().x() / imager->carreaux().x());
    m_cellsTy->setValue(imager->getRealSize().y() / imager->carreaux().y());

    connect(m_gridOK, &QPushButton::pressed, [=](){updateGrid(scene);});

}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::setEllipse(RegulEllipseItem *ellipse)
{
    m_currentItem = ellipse;

    if (m_gridPage)
        m_tabs->setTabVisible(m_tabs->indexOf(m_gridPage), false);

    if (m_lineDispositionPage && m_lineStylePage) {
        m_tabs->setTabVisible(m_tabs->indexOf(m_lineDispositionPage), false);
        m_tabs->setTabVisible(m_tabs->indexOf(m_lineStylePage), false);
    }
    if (!m_ellipseDispositionPage || !m_ellipseStylePage) {
        ellipseDispositionPage();
        ellipseStylePage();
        m_tabs->addTab(m_ellipseDispositionPage, tr("Disposition"));
        m_tabs->addTab(m_ellipseStylePage, tr("Style"));
    }
    m_tabs->setTabVisible(m_tabs->indexOf(m_ellipseDispositionPage), true);
    m_tabs->setTabVisible(m_tabs->indexOf(m_ellipseStylePage), true);

    QRectF ellipseRect = ellipse->mapRectToScene(ellipse->rect());

    QPointF center = ellipseRect.center();
    qreal xc = center.x();
    qreal yc = center.y();

    m_ellipseProp = ellipseRect.width() / ellipseRect.height();

    setEllipseUnits(m_unitCurrent);

    m_ellipseX->setValue(xc / m_units.x());
    m_ellipseY->setValue(yc / m_units.y());
    m_ellipseWidth->setValue(ellipseRect.width() /   m_units.x());
    m_ellipseHeight->setValue(ellipseRect.height() / m_units.y());

    connect(m_ellipseX, &QDoubleSpinBox::valueChanged, this, &RegulToolBox::updateEllipse);
    connect(m_ellipseY, &QDoubleSpinBox::valueChanged, this, &RegulToolBox::updateEllipse);
    connect(m_ellipseWidth, &QDoubleSpinBox::valueChanged, this, &RegulToolBox::updateEllipse);
    connect(m_ellipseHeight, &QDoubleSpinBox::valueChanged, this, &RegulToolBox::updateEllipse);

    connect(m_ellipseRotW, &QDial::valueChanged, this, &RegulToolBox::rotateEllipse);
    connect(m_ellipseRotS, &QDoubleSpinBox::valueChanged, this, &RegulToolBox::rotateEllipse);

    connect(m_ellipseClip, &QCheckBox::toggled, this, &RegulToolBox::setClip);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::setLine(RegulLineItem *ligne)
{
    m_currentItem = ligne;

    if (m_gridPage)
        m_tabs->setTabVisible(m_tabs->indexOf(m_gridPage), false);

    if (m_ellipseDispositionPage && m_ellipseStylePage) {
        m_tabs->setTabVisible(m_tabs->indexOf(m_ellipseDispositionPage), false);
        m_tabs->setTabVisible(m_tabs->indexOf(m_ellipseStylePage), false);
    }
    if (!m_lineDispositionPage || !m_lineStylePage) {
        lineDispositionPage();
        lineStylePage();
        m_tabs->addTab(m_lineDispositionPage, tr("Disposition"));
        m_tabs->addTab(m_lineStylePage, tr("Style"));
    }

    m_tabs->setTabVisible(m_tabs->indexOf(m_lineDispositionPage), true);
    m_tabs->setTabVisible(m_tabs->indexOf(m_lineStylePage), true);

    setLineUnits(m_unitCurrent);
    QPointF beg = ligne->mapToScene(ligne->line().x1(), ligne->line().y1());
    QPointF end = ligne->mapToScene(ligne->line().x2(), ligne->line().y2());

    m_lineBegValX->setValue(beg.x() / m_units.x());
    m_lineBegValY->setValue(beg.y() / m_units.y());
    m_lineEndValX->setValue(end.x() / m_units.x());
    m_lineEndValY->setValue(end.y() / m_units.y());

    connect(m_lineBegValX, &QDoubleSpinBox::valueChanged, this, &RegulToolBox::updateLine);
    connect(m_lineBegValY, &QDoubleSpinBox::valueChanged, this, &RegulToolBox::updateLine);
    connect(m_lineEndValX, &QDoubleSpinBox::valueChanged, this, &RegulToolBox::updateLine);
    connect(m_lineEndValY, &QDoubleSpinBox::valueChanged, this, &RegulToolBox::updateLine);

    connect(m_lineColor, &QPushButton::pressed, this, &RegulToolBox::updateLine);
    connect(m_lineThickness, &QSpinBox::valueChanged, this, &RegulToolBox::updateLine);

    connect(m_lineRotW, &QDial::valueChanged, this, &RegulToolBox::rotateLine);
    connect(m_lineRotS, &QDoubleSpinBox::valueChanged, this, &RegulToolBox::rotateLine);

    connect(m_lineStyleBox, &QComboBox::currentIndexChanged, [=](int index){setLineStyle(index);});
}

///////////////////////////////////////////////////////////////////////////////////////////
QPointF RegulToolBox::units() const
{
    QPointF rv(1.0, 1.0);
    if (!m_currentItem)
        return rv ;
    RegulScene * scene = dynamic_cast<RegulScene*>(m_currentItem->scene());
    rv.setX(m_units.x() / scene->tabSize().x());
    rv.setY(m_units.y() / scene->tabSize().y());
    return rv;
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::rotateEllipse()
{
    if(!m_currentItem->isSelected())
        return;
    QString classname(sender()->metaObject()->className());
    if (classname == "QDial")
        m_ellipseRotS->setValue(m_ellipseRotW->value());
    RegulEllipseItem *ellipse = dynamic_cast<RegulEllipseItem*>(m_currentItem);
    QRectF ellipseRect =  ellipse->rect();
    ellipse->setTransformOriginPoint(ellipseRect.center());
    ellipse->setRotation(m_ellipseRotS->value());

}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::rotateLine()
{
    if(!m_currentItem->isSelected())
        return;
    QString classname(sender()->metaObject()->className());
    if (classname == "QDial")
        m_lineRotS->setValue(m_lineRotW->value());
    RegulLineItem* line = dynamic_cast<RegulLineItem*>(m_currentItem);
    line->setTransformOriginPoint(line->line().center());
    line->setRotation(m_lineRotS->value());
}

////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::setClip()
{
    dynamic_cast<RegulEllipseItem*>(m_currentItem)->setClip(m_ellipseClip->isChecked());
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::setEllipseUnits(RegulToolBox::UnitType index)
{
    RegulScene * scene = dynamic_cast<RegulScene*>(m_currentItem->scene());
    RegulImager * imager = scene->viewer()->imager();
    qreal posx = m_ellipseX->value() * m_units.x();
    qreal posy = m_ellipseY->value() * m_units.y();

    qreal tailleW = m_ellipseWidth->value()  * m_units.x();
    qreal tailleH = m_ellipseHeight->value() * m_units.y();

    switch (index) {
    case kWH :{
        m_units = scene->tabSize();
        m_unitPerCentWHActE->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
        m_unitPerCentCarreauActE->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
        m_unitAbsoluteActE->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
        m_unitsButtonE->setText(m_unitPerCentWHActE->text());
        m_unitCurrent = kWH;
        break;
    }
    case kCARREAU: {
        if (imager->isCarreauxOn()){

            m_units.setX(scene->tabSize().x() / imager->carreaux().x());
            m_units.setY(scene->tabSize().y() / imager->carreaux().y());
            m_unitPerCentWHActE->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
            m_unitPerCentCarreauActE->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
            m_unitAbsoluteActE->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
            m_unitsButtonE->setText(m_unitPerCentCarreauActE->text());
            m_unitCurrent = kCARREAU;
        }
        break;
    }
    case kABS: {
        m_units.setX(scene->tabSize().x() / imager->getRealSize().x());
        m_units.setY(scene->tabSize().y() / imager->getRealSize().y());
        m_unitPerCentWHActE->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
        m_unitPerCentCarreauActE->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
        m_unitAbsoluteActE->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
        m_unitsButtonE->setText(m_unitAbsoluteActE->text());
        m_unitCurrent = kABS;
        break;
    }
    default:
        break;
    }
    m_ellipseX->setValue(posx / m_units.x());
    m_ellipseY->setValue(posy / m_units.y());

    m_ellipseWidth->setValue(tailleW / m_units.x());
    m_ellipseHeight->setValue(tailleH / m_units.y());

    imager->setUnits(units());
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::setLineStyle(int index)
{
    if(!m_currentItem->isSelected())
        return;
    Qt::PenStyle sty = static_cast<Qt::PenStyle>(index + 1);
    RegulEllipseItem *ellipse = dynamic_cast<RegulEllipseItem*>(m_currentItem);
    RegulLineItem *line = dynamic_cast<RegulLineItem*>(m_currentItem);
    if (line != nullptr) {
        QPen mypen = line->pen();
        mypen.setStyle(sty);
        line->setPen(mypen);
    }
    else if(ellipse != nullptr) {
        QPen mypen = ellipse->pen();
        mypen.setStyle(sty);
        ellipse->setPen(mypen);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::setLineUnits(UnitType index)
{
    RegulScene * scene = dynamic_cast<RegulScene*>(m_currentItem->scene());
    RegulImager * imager = scene->viewer()->imager();
    qreal x1 = m_lineBegValX->value() * m_units.x();
    qreal y1 = m_lineBegValY->value() * m_units.y();
    qreal x2 = m_lineEndValX->value() * m_units.x();
    qreal y2 = m_lineEndValY->value() * m_units.y();

    switch (index) {
    case kWH :{
        m_units = scene->tabSize();
        m_unitPerCentWHActL->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
        m_unitPerCentCarreauActL->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
        m_unitAbsoluteActL->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
        m_unitsButtonL->setText(m_unitPerCentWHActL->text());
        m_unitCurrent = kWH;
        break;
    }
    case kCARREAU: {
        if (imager->isCarreauxOn()){

            m_units.setX(scene->tabSize().x() / imager->carreaux().x());
            m_units.setY(scene->tabSize().y() / imager->carreaux().y());
            m_unitPerCentWHActL->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
            m_unitPerCentCarreauActL->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
            m_unitAbsoluteActL->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
            m_unitsButtonL->setText(m_unitPerCentCarreauActL->text());
            m_unitCurrent = kCARREAU;
        }
        break;
    }
    case kABS: {
        qDebug() << Q_FUNC_INFO << imager->getRealSize();
        m_units.setX(scene->tabSize().x() / imager->getRealSize().x());
        m_units.setY(scene->tabSize().y() / imager->getRealSize().y());
        m_unitPerCentWHActL->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
        m_unitPerCentCarreauActL->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
        m_unitAbsoluteActL->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
        m_unitsButtonL->setText(m_unitAbsoluteActL->text());
        m_unitCurrent = kABS;
        break;
    }
    default:
        break;
    }
    m_lineBegValX->setValue(x1 / m_units.x());
    m_lineBegValY->setValue(y1 / m_units.y());
    m_lineEndValX->setValue(x2 / m_units.x());
    m_lineEndValY->setValue(y2 / m_units.y());

    imager->setUnits(units());
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::updateEllipse()
{
    if(!m_currentItem->isSelected())
        return;

//    dynamic_cast<RegulEllipseItem*>(m_currentItem)->setClip(m_ellipseClip->isChecked());
    qreal xmax, ymax;
    RegulEllipseItem *ellipse = dynamic_cast<RegulEllipseItem*>(m_currentItem);
    RegulScene * scene = dynamic_cast<RegulScene*>(m_currentItem->scene());
    RegulImager * imager = scene->viewer()->imager();
    switch (m_unitCurrent) {
    case kWH: {
        xmax = 1.0;
        ymax = 1.0;
        break;
    }
    case kCARREAU : {
        xmax = imager->carreaux().x();
        ymax = imager->carreaux().y();
        break;
    }
    case kABS :{
        xmax = imager->getRealSize().x();
        ymax = imager->getRealSize().y();
        break;
    }
    }
    if (m_ellipseX->value() > xmax)
        m_ellipseX->setValue(xmax);
    if (m_ellipseY->value() > ymax)
        m_ellipseY->setValue(ymax);
    if (m_ellipseWidth->value() > 2 * xmax)
        m_ellipseWidth->setValue(2 * xmax);
    if (m_ellipseHeight->value() > 2 * ymax)
        m_ellipseHeight->setValue(2 * ymax);

    QPointF center = QPointF(m_ellipseX->value() * m_units.x(),
                             m_ellipseY->value() * m_units.y());

    QRectF ellipseRect =  ellipse->rect();
    ellipseRect.moveCenter(ellipse->mapFromScene(center));

    qreal width  = m_ellipseWidth->value()  * m_units.x();
    qreal height = m_ellipseHeight->value() * m_units.y();

    if (m_ellipseHW->isChecked()) {
            if (sender()->objectName() == "H") {
                width = height * m_ellipseProp;
                m_ellipseWidth->setValue(width / m_units.x());
            }
            else if(sender()->objectName() == "W") {
                height = width / m_ellipseProp;
                m_ellipseHeight->setValue(height / m_units.y());
            }
    }
    ellipseRect.setSize(QSizeF(width, height));
    ellipse->setRect(ellipseRect);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::updateGrid(RegulScene* scene)
{
    scene->updateCarreau((QPoint(m_cellsNx->value(), m_cellsNy->value())));
    RegulImager * imager = scene->viewer()->imager();
    imager->setCarreaux(QPoint(m_cellsNx->value(), m_cellsNy->value()));
    m_cellsTx->setValue(imager->getRealSize().x() / m_cellsNx->value());
    m_cellsTy->setValue(imager->getRealSize().y() / m_cellsNy->value());
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulToolBox::updateLine()
{
    if (!m_currentItem->isSelected())
        return;
    qreal xmax, ymax;
    RegulScene * scene = dynamic_cast<RegulScene*>(m_currentItem->scene());
    RegulImager * imager = scene->viewer()->imager();
    switch (m_unitCurrent) {
    case kWH: {
        xmax = 1.0;
        ymax = 1.0;
        break;
    }
    case kCARREAU : {
        xmax = imager->carreaux().x();
        ymax = imager->carreaux().y();
        break;
    }
    case kABS :{
        xmax = imager->getRealSize().x();
        ymax = imager->getRealSize().y();
        break;
    }
    }
    if (m_lineBegValX->value() > xmax)
        m_lineBegValX->setValue(xmax);
    if (m_lineBegValY->value() > ymax)
        m_lineBegValY->setValue(ymax);
    if (m_lineEndValX->value() > xmax)
        m_lineEndValX->setValue(xmax);
    if (m_lineEndValY->value() > ymax)
        m_lineEndValY->setValue(ymax);

    RegulLineItem* ligne = dynamic_cast<RegulLineItem*>(m_currentItem);

    if (sender() == m_lineThickness) {
        QPen pen = ligne->pen();
        pen.setWidth(m_lineThickness->value());
        ligne->setPen(pen);
    } else if (sender() == m_lineColor) {
        QColor color = QColorDialog::getColor(QColorDialog::ShowAlphaChannel);
        QPen pen = ligne->pen();
        pen.setColor(color);
        ligne->setPen(pen);
    } else {
        qreal x1 = m_lineBegValX->value() * m_units.x();
        qreal y1 = m_lineBegValY->value() * m_units.y();
        qreal x2 = m_lineEndValX->value() * m_units.x();
        qreal y2 = m_lineEndValY->value() * m_units.y();

        QPointF beg = m_currentItem->mapFromScene(x1, y1);
        QPointF end = m_currentItem->mapFromScene(x2, y2);

        ligne->setLine(beg.x(), beg.y(), end.x(), end.y());
    }
}
