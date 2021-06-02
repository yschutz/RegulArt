#include "regulmainwindow.h"
#include "regulimager.h"
#include "regullineitem.h"
#include "regulellipseitem.h"
#include "regulitemgroup.h"
#include "regulscene.h"
#include "regultoolbox.h"

#include <QApplication>
#include <QClipboard>
#include <QColorSpace>
#include <QDir>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QGridLayout>
#include <QGroupBox>
#include <QImageReader>
#include <QImageWriter>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QStandardPaths>
#include <QStatusBar>
#include <QStyle>
#include <QToolBar>

//#if defined(QT_PRINTSUPPORT_LIB)
//#  include <QtPrintSupport/qtprintsupportglobal.h>

//#  if QT_CONFIG(printdialog)
#    include <QPrintDialog>
//#  endif
//#endif

///////////////////////////////////////////////////////////////////////////////////////////
RegulImager::RegulImager(QWidget *parent)
   : QMainWindow(parent), m_toolBox(nullptr)
{
    m_viewer = new RegulViewer(this);
    m_scrollArea = new QScrollArea;
    m_scrollArea->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_scrollArea->ensureWidgetVisible(m_viewer, 0, 0);
    m_scrollArea->setWidget(m_viewer);
    setCentralWidget(m_scrollArea);

    createActions();

    m_xValueBox = new QDoubleSpinBox(this);
    m_xValueBox->setPrefix("x : ");
    m_xValueBox->setRange(0.0, 300);
    m_xValueBox->setDecimals(3);
    m_xValueBox->setSingleStep(0.01);
    m_yValueBox = new QDoubleSpinBox(this);
    m_yValueBox->setPrefix("y : ");
    m_yValueBox->setRange(0.0, 300);
    m_yValueBox->setDecimals(3);
    m_yValueBox->setSingleStep(0.01);

    m_cursorInfoBox = new QGroupBox(tr("Curseur"), this);
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(m_xValueBox);
    hboxLayout->addWidget(m_yValueBox);
    m_cursorInfoBox->setLayout(hboxLayout);

    QHBoxLayout *statusWidgetLayout = new QHBoxLayout();
    statusWidgetLayout->addWidget(m_cursorInfoBox);

    m_cercleRValueBox = new QDoubleSpinBox(this);
    m_cercleRValueBox->setPrefix("r : ");
    m_cercleRValueBox->setRange(0.0, 600);
    m_cercleRValueBox->setDecimals(3);
    m_cercleRValueBox->setSingleStep(0.01);

    m_cercleXValueBox = new QDoubleSpinBox(this);
    m_cercleXValueBox->setPrefix("x : ");
    m_cercleXValueBox->setRange(0.0, 300);
    m_cercleXValueBox->setDecimals(3);
    m_cercleXValueBox->setSingleStep(0.01);

    m_cercleYValueBox = new QDoubleSpinBox(this);
    m_cercleYValueBox->setPrefix("y : ");
    m_cercleYValueBox->setRange(0.0, 300);
    m_cercleYValueBox->setDecimals(3);
    m_cercleYValueBox->setSingleStep(0.01);

    m_cercleInfoBox = new QGroupBox(tr("Cercle"), this);
    hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(m_cercleRValueBox);
    hboxLayout->addWidget(m_cercleXValueBox);
    hboxLayout->addWidget(m_cercleYValueBox);
    m_cercleInfoBox->setLayout(hboxLayout);

    statusWidgetLayout->addWidget(m_cercleInfoBox);

    m_ligneXbegValueBox = new QDoubleSpinBox(this);
    m_ligneXbegValueBox->setPrefix("x1 : ");
    m_ligneXbegValueBox->setRange(0.0, 300);
    m_ligneXbegValueBox->setDecimals(3);
    m_ligneXbegValueBox->setSingleStep(0.01);

    m_ligneYbegValueBox = new QDoubleSpinBox(this);
    m_ligneYbegValueBox->setPrefix("y1 : ");
    m_ligneYbegValueBox->setRange(0.0, 300);
    m_ligneYbegValueBox->setDecimals(3);
    m_ligneYbegValueBox->setSingleStep(0.01);

    m_ligneXendValueBox = new QDoubleSpinBox(this);
    m_ligneXendValueBox->setPrefix("x2 : ");
    m_ligneXendValueBox->setRange(0.0, 300);
    m_ligneXendValueBox->setDecimals(3);
    m_ligneXendValueBox->setSingleStep(0.01);
    m_ligneYendValueBox = new QDoubleSpinBox(this);
    m_ligneYendValueBox->setPrefix("y2 : ");
    m_ligneYendValueBox->setRange(0.0, 300);
    m_ligneYendValueBox->setDecimals(3);
    m_ligneYendValueBox->setSingleStep(0.01);

    m_ligneInfoBox = new QGroupBox(tr("Ligne"), this);
    hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(m_ligneXbegValueBox);
    hboxLayout->addWidget(m_ligneYbegValueBox);
    hboxLayout->addWidget(m_ligneXendValueBox);
    hboxLayout->addWidget(m_ligneYendValueBox);
    m_ligneInfoBox->setLayout(hboxLayout);

    statusWidgetLayout->addWidget(m_ligneInfoBox);

    QWidget *statusWidget = new QWidget(this);
    statusWidget->setLayout(statusWidgetLayout);
    statusBar()->addWidget(statusWidget, Qt::AlignHCenter);

    connect(m_viewer, &RegulViewer::mouseMoved, this, &RegulImager::updateCursorPosValue);
    connect(m_viewer, &RegulViewer::cercleChanged, [=](bool show){RegulImager::updateCercleValues(show);});
    connect(m_viewer, &RegulViewer::ligneChanged,  [=](bool show){RegulImager::updateLigneValues(show);});

    connect(m_xValueBox, &QDoubleSpinBox::valueChanged, this, &RegulImager::updateCrossHairPos);
    connect(m_yValueBox, &QDoubleSpinBox::valueChanged, this, &RegulImager::updateCrossHairPos);

    m_cercleInfoBox->hide();
    m_ligneInfoBox->hide();


    QScreen *screen = QGuiApplication::primaryScreen();
    int minh = screen->availableSize().height() - 100;
    int minw = screen->availableSize().width() / 3;
    setMinimumWidth(minw);
    setMinimumHeight(minh);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::init()
{
    m_carreauxH = m_carreauxW = 0;
    m_cercleRValueBox->setValue(0.0);
    m_cercleXValueBox->setValue(0.0);
    m_cercleYValueBox->setValue(0.0);

    m_image     = QImage();
    m_imageNetB = QImage();

    m_ligneXbegValueBox->setValue(0.0);
    m_ligneXendValueBox->setValue(0.0);
    m_ligneYbegValueBox->setValue(0.0);
    m_ligneYendValueBox->setValue(0.0);

    m_nomTableau.clear();
    m_nomPeintre.clear();

    m_originalHeight = m_originalWidth = 0.0;

    m_viewer->init();

    m_scaleH = m_scaleW = 1.0;

    m_xValueBox->setValue(0.0);
    m_yValueBox->setValue(0.0);

    m_carreauxOn = m_crayonOn = m_crossHairOn = false;
    m_diagoOn = m_ellipseOn = m_noirEtBlancOn = m_viewOn = false;

    m_carreauxAct->setChecked(false);
    m_crayonAct->setChecked(false);
    m_crosshairAct->setChecked(false);
    m_diagoAct->setChecked(false);
    m_ellipseAct->setChecked(false);
    m_ellipseAct->setChecked(false);

    setTraceSaved(false);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::assignToolBox()
{
    QList<QGraphicsItem*>  itList = m_viewer->scene()->items();
    for (QGraphicsItem *it : itList) {
            switch (it->type()) {
//            case RegulItemGroup::Type: {
//                m_toolBox->setGrid(theView()->scene());
//                break;
//            }
            case RegulLineItem::Type: {
                RegulLineItem* rit = dynamic_cast<RegulLineItem*>(it);
                if (it->isSelected()) m_toolBox->setLine(rit);
                break;
            }
            case RegulEllipseItem::Type: {
                RegulEllipseItem* rit = dynamic_cast<RegulEllipseItem*>(it);
                if (it->isSelected()) m_toolBox->setEllipse(rit);
                break;
            }
            }
    }
    m_toolBox->show();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::carreau()
{
    if (!m_carreauxOn) {
        m_viewer->miseAuCarreau(true, m_carreauxW, m_carreauxH);
        m_carreauxOn = true;
    } else {
        m_viewer->miseAuCarreau(false);
        m_carreauxOn = false;
        m_carreauxAct->setChecked(false);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::close()
{
    if (!m_traceSaved && m_viewer->scene()->items().size() > 1) {
        QMessageBox msg;
        msg.setStandardButtons(QMessageBox::Yes);
        msg.setButtonText(QMessageBox::Yes, "Oui");
        msg.addButton(QMessageBox::No);
        msg.setButtonText(QMessageBox::No, "Non");
        msg.setDefaultButton(QMessageBox::No);
        msg.setText("Sauvegarder le tracé ? ...");
        if (msg.exec() == QMessageBox::Yes) {
            (dynamic_cast<RegulMainWindow*>(parent()))->save();
        }
         else {
            setTraceSaved(true);
            QWidget::close();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::crayon()
{
    if (!m_crayonOn) {
        m_viewer->dessineLigne(true);
        m_crayonOn = true;
        m_ellipseOn =  m_viewOn = false;
        m_ellipseAct->setChecked(false);
        m_ellipseAct->setEnabled(false);
        m_viewAct->setChecked(false);
        m_ligneInfoBox->show();
        m_cercleInfoBox->hide();
    } else {
        m_viewer->dessineLigne(false);
        m_crayonOn = false;
        m_crayonAct->setChecked(false);
        m_ellipseAct->resetEnabled();
        m_ligneInfoBox->hide();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::createActions()
{
    QStyle * style = qApp->style();
    m_tools = addToolBar("the tools");

    QIcon closeIco = style->standardIcon(QStyle::SP_TitleBarCloseButton);
    QAction *exitAct = m_tools->addAction(closeIco, tr("E&xit"), this, &RegulImager::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QIcon zoominIco = QIcon(":/icons/zoomin.png");
    m_zoomInAct = m_tools->addAction(zoominIco, tr("Zoom &In (25%)"), [this](){RegulImager::zoomIn();});
    m_zoomInAct->setShortcut(QKeySequence::ZoomIn);
    m_zoomInAct->setEnabled(true);

    QIcon zoomoutIco = QIcon(":/icons/zoomout.png");
    m_zoomOutAct = m_tools->addAction(zoomoutIco, tr("Zoom &Out (25%)"), [this](){RegulImager::zoomOut();});
    m_zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    m_zoomOutAct->setEnabled(true);

    QIcon schrinkIco = QIcon(":/icons/schrink.png");
    m_normalSizeAct = m_tools->addAction(schrinkIco, tr("&Normal Size"), this, &RegulImager::fitSize);
    m_normalSizeAct->setShortcut(tr("Ctrl+S"));
    m_normalSizeAct->setEnabled(true);

    QIcon expandIco = QIcon(":/icons/expand.png");
    m_fitSizeAct = m_tools->addAction(expandIco, tr("&Adapte taille"), this, &RegulImager::normalSize);
    m_fitSizeAct->setShortcut(tr("Ctrl+S"));
    m_fitSizeAct->setEnabled(true);


    QIcon imageOffsetIco = QIcon(":/icons/XY-Offset-Directions.png");
    m_imageOffsetAct = m_tools->addAction(imageOffsetIco, tr("Recadrer l'image"), this, &RegulImager::imageOffset);
    m_imageOffsetAct->setShortcut(tr("Ctrl+O"));
    m_imageOffsetAct->setEnabled(true);
    m_imageOffsetAct->setCheckable(true);

    QIcon noirEtBlancIco = QIcon(":/icons/BetW.png");
    m_noirEtBlancAct = m_tools->addAction(noirEtBlancIco, tr("afficher en noir et blanc"), this, &RegulImager::noiretBlanc);
    m_noirEtBlancAct->setShortcut(tr("Ctrl+N"));
    m_noirEtBlancAct->setEnabled(true);
    m_noirEtBlancAct->setCheckable(true);

    QIcon crosshairIco = QIcon(":/icons/crosshair.png");
    m_crosshairAct = m_tools->addAction(crosshairIco, tr("Activer le curseur"), this, &RegulImager::crossHair);
    m_crosshairAct->setShortcut(tr("Ctrl+H"));
    m_crosshairAct->setEnabled(true);
    m_crosshairAct->setCheckable(true);

    QIcon carreauxIco = QIcon(":/icons/carreaux.png");
    m_carreauxAct = m_tools->addAction(carreauxIco, tr("Mise au carreau"), this, &RegulImager::carreau);
    m_carreauxAct->setShortcut(tr("Ctrl+C"));
    m_carreauxAct->setEnabled(true);
    m_carreauxAct->setCheckable(true);

    QIcon diagonalIco = QIcon(":/icons/diagonal.png");
    m_diagoAct = m_tools->addAction(diagonalIco, tr("tracer des diagonales"), this, &RegulImager::diagonales);
    m_diagoAct->setShortcut(tr("Ctrl+D"));
    m_diagoAct->setEnabled(true);
    m_diagoAct->setCheckable(true);

    QIcon crayonIco = QIcon(":/icons/pen.png");
    m_crayonAct = m_tools->addAction(crayonIco, tr("tracer ligne"), this, &RegulImager::crayon);
    m_crayonAct->setShortcut(tr("Ctrl+P"));
    m_crayonAct->setEnabled(true);
    m_crayonAct->setCheckable(true);

    QIcon ellipseIco = QIcon(":/icons/ellipse.png");
    m_ellipseAct = m_tools->addAction(ellipseIco, tr("tracer ellipse"), this, &RegulImager::ellipse);
    m_ellipseAct->setShortcut(tr("Ctrl+E"));
    m_ellipseAct->setEnabled(true);
    m_ellipseAct->setCheckable(true);

    QIcon viewIco = QIcon(":/icons/view.png");
    m_viewAct = m_tools->addAction(viewIco, tr("tous les tracés"), this, &RegulImager::view);
    m_viewAct->setShortcut(tr("Ctrl+V"));
    m_viewAct->setEnabled(true);
    m_viewAct->setCheckable(true);

    QIcon toolIco = QIcon(":/icons/toolbox.png");
    m_toolBoxAct = m_tools->addAction(toolIco, tr("afficher boite à outils"), this, &RegulImager::toolBox);
    m_toolBoxAct->setShortcut(tr("Ctrl+T"));
    m_toolBoxAct->setEnabled(true);
    m_toolBoxAct->setCheckable(true);

    QIcon texlIco = QIcon(":/icons/tex.png");
    m_texAct = m_tools->addAction(texlIco, tr("sauvegarder TeX/tikz"), this, &RegulImager::texIt);
    m_texAct->setShortcut(tr("Ctrl+X"));
    m_texAct->setEnabled(true);
    m_texAct->setCheckable(false);

}


///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::crossHair()
{
    if (!m_crossHairOn){
        m_viewer->activateCrossHair(true);
        m_crossHairOn = true;
    } else {
        m_viewer->activateCrossHair(false);
        m_crossHairOn = false;
        m_crosshairAct->setChecked(false);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::diagonales()
{
    if (!m_diagoOn) {
        m_viewer->traceDiagonales(true);
        m_diagoOn = true;
    } else {
        m_viewer->traceDiagonales(false);
        m_diagoOn = false;
        m_diagoAct->setChecked(false);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::ellipse()
{
    if (!m_ellipseOn) {
        m_viewer->dessineEllipse(true);
        m_ellipseOn = true;
        m_crayonOn  = m_viewOn = false;
        m_crayonAct->setChecked(false);
        m_crayonAct->setEnabled(false);
        m_cercleInfoBox->show();
        m_ligneInfoBox->hide();
    } else {
        m_viewer->dessineEllipse(false);
        m_ellipseOn = false;
        m_ellipseAct->setChecked(false);
        m_crayonAct->resetEnabled();
        m_cercleInfoBox->hide();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::fitSize()
{
    m_viewer->resetTransform();
    m_viewer->scale(qMin(m_scaleW, m_scaleH), qMin(m_scaleW, m_scaleH));
    zoomOut();
    zoomOut();
    m_scrollArea->horizontalScrollBar()->setValue(m_scrollArea->horizontalScrollBar()->maximum() / 2);
    m_scrollArea->verticalScrollBar()->setValue(m_scrollArea->verticalScrollBar()->maximum() / 2);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::imageOffset()
{
}


///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::loadImage(const QString &imageFileName)
{
    QStringList l = imageFileName.split("/");
    m_nomTableau = l.at(l.size() - 1);
    m_nomTableau =  m_nomTableau.left(m_nomTableau.indexOf("."));
    m_nomPeintre = l.at(l.size() - 2);
    const QString message(QString("%1 de %2, lxh = %3 x %4").arg(m_nomTableau).arg(m_nomPeintre).arg(m_originalWidth).arg(m_originalHeight));
    setWindowTitle(message);

    int scaleFactor = 2;
    QImageReader reader(imageFileName);
    QSize imageSize = reader.size();
    QSize size(imageSize.width() / scaleFactor, imageSize.height() / scaleFactor);
    reader.setScaledSize(size);
    reader.setAutoTransform(true);
    m_image = reader.read();
    if (m_image.colorSpace().isValid())
        m_image.convertToColorSpace(QColorSpace::SRgb);

    qreal scaleT = 1.0 * imageSize.height() / imageSize.width();
    qreal scaleO =  m_originalHeight / m_originalWidth;
    qreal diff = 100 * (scaleT - scaleO) / scaleO;

    qreal newWidth[2], newHeight[2];
    newWidth[0]  = size.width();
    newHeight[0] = size.width() * scaleO;
    newWidth [1] = size.height() / scaleO;
    newHeight[1] = size.height();

//    qreal aspectRatioDiff = qAbs(newHeight[0] / newWidth[0] - scaleO);
    QSize scaledSize(imageSize);
    QMessageBox msgBox(this);
    msgBox.setText(QString("Les rapports d'aspect entre le tableau original et son image diffèrent de %1 %.").arg(diff, 3, 'g', 2));
//    if (qAbs(newHeight[1] / newWidth[1] - scaleO) > aspectRatioDiff) {
//        scaledSize = QSize(newWidth[0], static_cast<int>(newHeight[0] + 0.5));
//        msgBox.setText(msgBox.text() + "\n image allongée");
//    }
//    else {
//        scaledSize = QSize(static_cast<int>(newWidth[1] + 0.5), newHeight[1]);
//        msgBox.setText(msgBox.text() + "\n image élargie");
//    };
    msgBox.exec();

//    m_image = m_image.scaled(scaledSize, Qt::IgnoreAspectRatio);

    m_imageNetB = m_image;
    m_imageNetB.convertTo(QImage::Format_Grayscale16);

    miseAuCarreaux(scaledSize);

    m_viewer->loadImage(m_image);

    m_scaleW = 1.0 * width() / m_viewer->dimension().width();
    m_scaleH = 1.0 * height() / m_viewer->dimension().height();
    m_viewer->resetTransform();
    m_viewer->scale(qMin(m_scaleW, m_scaleH), qMin(m_scaleW, m_scaleH));
    zoomOut();
    zoomOut();
    show();

    m_scrollArea->horizontalScrollBar()->setValue(m_scrollArea->horizontalScrollBar()->maximum() / 2);
    m_scrollArea->verticalScrollBar()->setValue(m_scrollArea->verticalScrollBar()->maximum() / 2);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::miseAuCarreaux(const QSize &size)
{
    int w = size.width();
    int h = size.height();
    qreal longest     = qMax(w, h);
    qreal shortest    = qMin(w, h);

    qreal epsilon = 1.0;
    for (int nShortest = 2; nShortest < 20; ++nShortest) {
        qreal sizeSquare = shortest / nShortest;
        qreal nLongest = longest / sizeSquare;
        int    inLongest = qFloor(nLongest);
        qreal rnLongest = nLongest - inLongest;
        if (rnLongest < epsilon){
            epsilon = rnLongest;
            if ( w == qMin(w, h)){
                m_carreauxW = qMin(nShortest, inLongest);
                m_carreauxH = qMax(nShortest, inLongest);
            } else {
                m_carreauxH = qMin(nShortest, inLongest);
                m_carreauxW = qMax(nShortest, inLongest);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::noiretBlanc()
{
    if (!m_noirEtBlancOn) {
        m_viewer->setNetB(true);
        m_noirEtBlancOn = true;
    } else {
        m_viewer->setNetB(false);
        m_noirEtBlancOn = false;
        m_noirEtBlancAct->setChecked(false);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::normalSize()
{
    m_viewer->resetTransform();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::texIt()
{
    QPointF offset = theView()->scene()->ImageOffset();
    offset.setX(offset.x() / m_image.size().width());
    offset.setY(-offset.y() / m_image.size().height());
    QString path = static_cast<RegulMainWindow *>(this->parent())->pathName();
    path.append("tex/");
    QDir texDir;
    texDir.setPath(QDir::homePath() + path);
    if (!texDir.exists())
        texDir.mkpath(".");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Enregistrer fichier TeX"),
                                texDir.path(),
                                tr("Text files (*.tex)"));
    QFile outFile(fileName);
    outFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&outFile);
    out << "\\begin{figure}\n";
    out << "    \\centering\n";
    out << "    \\begin{tikzpicture}\n";
    out << "		\\pgfmathsetmacro\\w{0.8\\textwidth)/28.45276};\n";
    out << "%        \\pgfmathsetmacro\\h{0.8\\textheight)/28.45276};\n";
    out << QString("        \\pgfmathsetmacro\\nr{%1 / %2};\n").arg(m_originalHeight).arg(m_originalWidth);
    out << "		\\pgfmathsetmacro\\h{\\w * \\nr};\n";
    out << "%        \\pgfmathsetmacro\\w{\\h / \\nr};\n";
    out << "        \\pgfmathsetmacro\\nw{\\w / 8};\n";
    out << "        \\pgfmathsetmacro\\nh{\\h / 11};\n";
    out << "        \\pgfmathsetmacro\\D{sqrt(\\w * \\w + \\h * \\h) / 2}\n";
    out << "        \\pgfmathsetmacro\\dh{sqrt(\\w * \\w + \\h / 2 * \\h / 2) / 2};\n";
    out << "        \\pgfmathsetmacro\\dw{sqrt(\\w / 2 * \\w / 2 + \\h * \\h) / 2};\n";
    out << QString("        \\pgfmathsetmacro\\ox{%1 * \\w};\n").arg(offset.x());
    out << QString("        \\pgfmathsetmacro\\oy{%1 * \\h};\n").arg(offset.y());
    out << QString("        \\node[anchor=south west,inner sep=0] (image) at (\\ox, \\oy) {\\includegraphics[keepaspectratio=true,width=0.8\\textwidth]{%3}};\n").arg(m_nomTableau);
    out << "        %rectangle de l'image\n";
    out << "        \\draw[white, ultra thick](0, 0) rectangle(\\w, \\h);\n";
    //
    QString titre;
    if (m_diagoAct->isChecked()) {
        out << "        \\draw[blue!20, thin] (0, 0) to (\\w, \\h);\n";
        out << "        \\draw[blue!20, thin] (0, 0) to (\\w, 0.5 * \\h)node[black, right]{$\\frac{1}{2}$};\n";
        out << "        \\draw[blue!20, thin] (0, 0) to (0.5 * \\w, \\h);\n";
        out << "%\n";
        out << "        \\draw[blue!20, thin] (\\w, 0) to (0, \\h);\n";
        out << "        \\draw[blue!20, thin] (\\w, 0) to (0, 0.5 * \\h);\n";
        out << "        \\draw[blue!20, thin] (\\w, 0) to (0.5 * \\w, \\h);\n";
        out << "%\n";
        out << "        \\draw[blue!20, thin] (\\w, \\h) to (0, 0.5 * \\h);\n";
        out << "        \\draw[blue!20, thin] (\\w, \\h) to (0.5 * \\w, 0) node[black, below]{$\\frac{1}{2}$};\n";
        out << "%\n";
        out << "        \\draw[blue!20, thin] (0, \\h) to (\\w, 0.5 * \\h);\n";
        out << "        \\draw[blue!20, thin] (0, \\h) to (0.5 * \\w, 0);\n";
        out << "%\n";
        out << "        \\draw[blue!20, thin] (0, 0.5 * \\h ) to (\\w, 0.5 * \\h);\n";
        out << "        \\draw[blue!20, thin] (0.5 * \\w, 0 ) to (0.5 * \\w, \\h);\n";
        out << "%\n";
        out << "        \\draw[blue!20, thin] (0, 0.5 * \\h ) to (0.5 * \\w, \\h);\n";
        out << "        \\draw[blue!20, thin] (0, 0.5 * \\h ) to (0.5 * \\w, 0);\n";
        out << "%\n";
        out << "        \\draw[blue!20, thin] (\\w, 0.5 * \\h ) to (0.5 * \\w, \\h);\n";
        out << "        \\draw[blue!20, thin] (\\w, 0.5 * \\h ) to (0.5 * \\w, 0);\n";
        titre = "Rose des diagonales.";
    } else if (m_carreauxAct->isChecked()) {
        out << QString("        \\def\\nCw{%1};\n").arg(m_carreauxW);
        out << QString("        \\def\\nCh{%1};\n").arg(m_carreauxH);
        out << "        \\foreach \\nx in {1, 2, ...,\\nCw} {\n";
        out << "        	\\filldraw[black] (\\nx * \\nw, \\h) circle (2pt) node[anchor=south] {\\nx};\n";
        out << "        	\\draw[blue!20, thin] (\\nx * \\nw, \\h) to (\\nx * \\nw, 0);\n";
        out << "        }\n";
        out << "        \\foreach \\ny in {0,1,...,\\nCh} {\n";
        out << "        	\\pgfmathsetmacro\\nyy{int(\\nCh - \\ny)};\n";
        out << "        	\\filldraw[black] (0, \\ny * \\nh) circle (2pt) node[anchor=east] {\\nyy};\n";
        out << "        	\\draw[blue!20, thin] (0, \\ny * \\nh) to (\\w, \\ny * \\nh);\n";
        out << "        }\n";
        titre = "Mise au carreau.";
    }
    QListIterator<QGraphicsItem*> it(theView()->scene()->items());
    int nLcoor = 0;
    int nEcoor = 0;
     while (it.hasNext()) {
        QGraphicsItem * item = it.next();
        if (item->type() == RegulLineItem::Type) {
            RegulLineItem * litem = dynamic_cast<RegulLineItem*>(item);
            if (litem->isInGroup()) continue;
            QPointF beg = litem->mapToScene(litem->line().x1(), litem->line().y1());
            QPointF end = litem->mapToScene(litem->line().x2(), litem->line().y2());
            qreal begx = beg.x() / theView()->scene()->tabSize().x();
            qreal begy = 1 - beg.y() / theView()->scene()->tabSize().y();
            qreal endx = end.x() / theView()->scene()->tabSize().x();
            qreal endy = 1 - end.y() / theView()->scene()->tabSize().y();
            int red   = qRed(litem->pen().color().rgba());
            int green = qGreen(litem->pen().color().rgba());
            int blue  = qBlue(litem->pen().color().rgba());
            int alpha = qAlpha(litem->pen().color().rgba());
            QString scolor = QString("color={rgb:red, %1; green, %2; blue, %3}, opacity = %4").arg(red).arg(green).arg(blue).arg(alpha / 255);
            QString swidth;
            switch (litem->pen().width()) {
            case 1: {
                swidth = "ultra thin";
                break;
            }
            case 2: {
                swidth = "very thin";
                break;
            }
            case 3: {
                swidth = "thin";
                break;
            }
            case 4: {
                swidth = "thick";
                break;
            }
            case 5: {
                swidth = "very thick";
                break;
            }
            case 6: {
                swidth = "ultra thick";
                break;
            }
            }
            nLcoor++;
            out << QString("        \\coordinate (C%1) at (%2 * \\w, %3 * \\h);\n").arg(nLcoor).arg(begx).arg(begy);
            nLcoor++;
            out << QString("        \\coordinate (C%1) at (%2 * \\w, %3 * \\h);\n").arg(nLcoor).arg(endx).arg(endy);
            out << QString("        \\draw[%1, %2] (C%3) to (C%4);\n").arg(scolor,swidth).arg(nLcoor).arg(nLcoor-1);

        } else if (item->type() == RegulEllipseItem::Type) {
            RegulEllipseItem* eitem = dynamic_cast<RegulEllipseItem*>(item);
            QPointF center = eitem->mapToScene(eitem->rect().center());
            QPointF radius = eitem->mapToScene(eitem->radius());

            QPointF size = theView()->scene()->tabSize();
            QString scolor("black");
            if (center.x() > 0 && center.x() < size.x() && center.y() > 0 && center.y() < size.y())
                scolor = "white";
            nEcoor++;
            out << QString("        \\coordinate[label={[%1]below right:$C_%2$}](A%2) at (%3*\\w, %4*\\h);\n").arg(scolor).arg(nEcoor)
                                                                                                        .arg(center.x() / size.x())
                                                                                                        .arg((size.y() - center.y()) / size.y());
            out << QString("        \\filldraw[%1] (A%2) circle (2pt);\n").arg(scolor).arg(nEcoor);
            if (eitem->isInsideTab()) {
                out << QString("        \\coordinate(Ar%1) at (%2*\\w, %3*\\h);\n").arg(nEcoor)
                       .arg((center.x() + radius.x() ) / size.x())
                       .arg((size.y() - center.y()) / size.y());
                out << QString("        \\tkzDrawArc[rotate, color=blue!20, thick](A%1,Ar%1)(360);\n").arg(nEcoor);

            } else {
                QList<QPointF> inters = eitem->intersects();
                if (inters.size() == 2) {
                    QPointF p1 = inters[0];
                    QPointF p2 = inters[1];
                    qreal corde = qSqrt((p1.x() - p2.x()) * (p1.x() - p2.x()) + (p1.y() - p2.y()) * (p1.y() - p2.y()));
                    qreal py = -1, px = -1;
                    qreal spanAngle = 0;
                    if (p1.x() == 0 ) {
                        if (p2.x() == 0) {
                            px = 0;
                            py = p1.y();
                            spanAngle = 360 - qRadiansToDegrees(2 * qAsin(corde / 2 / eitem->radius().y()));
                        } else if (p2.y() == 0) {
                            px = 0;
                            py = p1.y();
                            spanAngle = qRadiansToDegrees(2 * qAsin(corde / 2 / eitem->radius().y()));
                        } else if (p2.y() == size.y()) {
                            px = p2.x();
                            py = size.y();
                            spanAngle = qRadiansToDegrees(2 * qAsin(corde / 2 / eitem->radius().y()));
                        } else if (p2.x() == size.x()) {
                            px = 0;
                            py = p1.y();
                            spanAngle = qRadiansToDegrees(2 * qAsin(corde / 2 / eitem->radius().y()));
                            if ((p1.y() < center.y()) && (p2.y() < center.y()))
                                spanAngle *= -1;
                        }
                    } else if (p1.x() == size.x()) {
                        if (p2.x() == size.x()) {
                            px = size.x();
                            py = p2.y();
                            spanAngle = 360 - qRadiansToDegrees(2 * qAsin(corde / 2 / eitem->radius().y()));
                        } else if (p2.y() == 0) {
                            px = p2.x();
                            py = 0;
                            spanAngle = qRadiansToDegrees(2 * qAsin(corde / 2 / eitem->radius().y()));
                        } else if (p2.y() == size.y()) {
                            px = p2.x();
                            py = size.y();
                            spanAngle = -qRadiansToDegrees(2 * qAsin(corde / 2 / eitem->radius().y()));
                        }
                    } else if (((p1.y() == 0) && (p2.y() == size.y())) || ((p1.y() == size.y()) && (p2.y() == 0)) ) {
                            px = p1.x();
                            py = p1.y();
                            spanAngle = qRadiansToDegrees(2 * qAsin(corde / 2 / eitem->radius().y()));
                            if ((p1.x() > center.x()) && (p2.x() > center.x()))
                                spanAngle *= -1;
                    }
                    out << QString("        \\coordinate(Ar%1) at (%2*\\w, %3*\\h);\n").arg(nEcoor)
                                                                                       .arg(px / size.x())
                                                                                       .arg((size.y() - py) / size.y());
                    out << QString("        \\tkzDrawArc[rotate, color=blue!20, thick](A%1,Ar%1)(%2);\n").arg(nEcoor)
                                                                                                         .arg(spanAngle);
                }
            }

        }
    }
    out << "        \\end{tikzpicture}\n";
    out << QString("    \\caption{\\\\ \\textbf{\\scshape {%1}} \\\\ %2}\n").arg(m_nomTableau,titre);
    out << QString("    \\label{fig:chapxx:%1}\n").arg(QString(m_nomTableau).remove(" "));
    out << "\\end{figure}\n";
    outFile.close();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::setRealSize(qreal w, qreal h)
{
    m_originalWidth  = w;
    m_originalHeight = h;
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::toolBox()
{
    if (m_toolBoxOn) {
        m_toolBox->hide();
        m_toolBoxAct->setChecked(false);
        m_toolBoxOn = false;
    } else {
        if (!m_toolBoxOn)
            m_toolBox = new RegulToolBox(this);
        assignToolBox();
        m_toolBoxOn = true;
        m_toolBoxAct->setChecked(true);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::updateCercleValues(bool show)
{
    if (show) {
        m_cercleInfoBox->show();
        qreal rw = m_viewer->cercleRadius().x() / m_units.x();
        qreal rh = m_viewer->cercleRadius().y() / m_units.y();
        qreal radius = sqrt(rw * rw + rh * rh);
        m_cercleRValueBox->setValue(radius);
        m_cercleXValueBox->setValue(m_viewer->cercleCenter().x() / m_units.x());
        m_cercleYValueBox->setValue(m_viewer->cercleCenter().y() / m_units.y());
    } else
        m_cercleInfoBox->hide();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::updateCursorPosValue()
{
    m_xValueBox->blockSignals(true);
    m_yValueBox->blockSignals(true);
    QPointF pos = dynamic_cast<RegulViewer*>(sender())->mousePos();
    m_xValueBox->setValue(pos.x() / m_units.x());
    m_yValueBox->setValue(pos.y() / m_units.y());
    m_xValueBox->blockSignals(false);
    m_yValueBox->blockSignals(false);
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::updateCrossHairPos()
{
    if(m_crossHairOn)  {
        m_viewer->updateCrossHairPosition(QPointF(m_xValueBox->value(), m_yValueBox->value()));
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::updateLigneValues(bool show)
{
    if(show) {
        m_ligneInfoBox->show();
        m_ligneXbegValueBox->setValue(m_viewer->ligneBeg().x() / m_units.x());
        m_ligneYbegValueBox->setValue(m_viewer->ligneBeg().y() / m_units.y());
        m_ligneXendValueBox->setValue(m_viewer->ligneEnd().x() / m_units.x());
        m_ligneYendValueBox->setValue(m_viewer->ligneEnd().y() / m_units.y());
    } else
        m_ligneInfoBox->hide();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::view()
{
    if (!m_viewOn) {
        m_viewer->showTraces(true);
        m_viewOn = true;
        m_ellipseOn = m_crayonOn = false;
        m_ellipseAct->setChecked(false);
        m_crayonAct->setChecked(false);
        m_ellipseAct->setEnabled(false);
        m_crayonAct->setEnabled(false);

    } else {
        m_viewer->showTraces(false);
        m_viewOn = false;
        m_viewAct->setChecked(false);
        m_ellipseAct->resetEnabled();
        m_crayonAct->resetEnabled();
    }

}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::keyPressed(QKeyEvent *evt)
{
    if (m_imageOffsetAct->isChecked()) {
        int fac = 1;
        if (evt->modifiers() & Qt::ShiftModifier)
            fac = 10;
        QPointF offset = m_viewer->scene()->ImageOffset();
        switch (evt->key()) {
        case Qt::Key_Left:
            offset.setX(offset.x() - 0.1 * m_kcm2pixel * fac);
            m_viewer->scene()->setImageOffset(offset);
            break;
        case Qt::Key_Right:
            offset.setX(offset.x() + 0.1 * m_kcm2pixel * fac);
            m_viewer->scene()->setImageOffset(offset);
            break;
        case Qt::Key_Up:
            offset.setY(offset.y() - 0.1 * m_kcm2pixel * fac);
            m_viewer->scene()->setImageOffset(offset);
            break;
        case Qt::Key_Down:
            offset.setY(offset.y() + 0.1 * m_kcm2pixel * fac);
            m_viewer->scene()->setImageOffset(offset);
            break;
        default:
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulImager::zoomIn()
{
    m_viewer->scale(1.25, 1.25);
}

void RegulImager::zoomOut()
{
    m_viewer->scale(0.9, 0.9);
}
