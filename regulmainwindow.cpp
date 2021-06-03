#include "regulmainwindow.h"
#include "regulitemgroup.h"
#include "regulellipseitem.h"
#include "regullabel.h"
#include "regullineitem.h"
#include "regulpushbutton.h"
#include "regulscene.h"
#include "regulstreamers.h"

#include <QFileDialog>
#include <QGraphicsItem>
#include <QGuiApplication>
#include <QImageReader>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QPrintDialog>
#include <QPushButton>
#include <QStandardPaths>
#include <QStatusBar>
#include <QToolBar>

///////////////////////////////////////////////////////////////////////////////////////////
 RegulMainWindow:: RegulMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
     m_mgr = new DownLoadManager(m_kPath, this);
     m_imagesDir = m_mgr->downloadDir();
     m_academie = new RegulAcademie(this);

     m_infoLabel = new RegulLabel("Sélectionnez un oeuvre à partir du menu principal");
     m_authorsViewLayout = new QHBoxLayout();
     m_authorsPage = new QWidget;

     m_authorsPage->setWindowTitle("Les peintres");
     m_authorsPage->setLayout(m_authorsViewLayout);

     m_pages = new QStackedWidget(this);
     m_pages->addWidget(m_authorsPage);


     setCentralWidget(m_pages);

     m_statusBarLabel = new QLabel(m_pages);
     m_statusBarLabel->setFont(QFont("HanziPen TC", 14, QFont::Bold));
     statusBar()->addWidget(m_statusBarLabel);

     QPixmap retour(":/icons/retour.png");
     QToolBar *toolbar = addToolBar("main toolbar");
     m_aRetour = toolbar->addAction(QIcon(retour), "Retour peintres");
     m_aRetour->setVisible(false);

     connect(m_aRetour, &QAction::triggered, this, & RegulMainWindow::returnFirstPage);

     buildMenu();

     displayAuthorsDirectory();

     m_imagesDir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
     QFileInfoList subdirs = m_imagesDir.entryInfoList();
     for (int index = 0; index < subdirs.size(); index++)
        displayTableauxDirectory(subdirs.at(index).fileName());

     setWindowTitle("RegulArt : Le Tracé Régulateur");
     QScreen *screen = QGuiApplication::primaryScreen();
     int minh = screen->availableSize().height() / 3;
     int minw = screen->availableSize().width() / 3;
     setMinimumWidth(minw);
     setMinimumHeight(minh);

//    resize( static_cast<int>(subdirs.size() * minimumWidth()), minimumHeight());
      show();

      m_downloadProgress = new QLabel("Téléchargement en cours ....");
      statusBar()->addWidget(m_downloadProgress);
      m_downloadProgress->hide();

      m_imager   = new RegulImager(this);

      connect(m_academie, &RegulAcademie::photoready,     this, & RegulMainWindow::updatePagePeintre);
      connect(m_academie, &RegulAcademie::tableauLDready, this, & RegulMainWindow::updatePageTableaux);
}

///////////////////////////////////////////////////////////////////////////////////////////
 RegulMainWindow::~ RegulMainWindow()
{
    m_imager->close();
}

///////////////////////////////////////////////////////////////////////////////////////////
void  RegulMainWindow::buildMenu()
{
    QIcon tableauIco = QIcon(":/icons/tableau.png");
    QMenu *menuPeintresTableaux = menuBar()->addMenu(tableauIco, tr("&Sélection Tableaux"));

    QListIterator<RegulPeintre*> itp(m_academie->peintres());
    QIcon peintreIco = QIcon(":/icons/peintre.png");
    while (itp.hasNext()) {
        RegulPeintre *pepe = itp.next();
        QString nomPeintre = pepe->nom();
        QMenu *menuPeintres = menuPeintresTableaux->addMenu(peintreIco, nomPeintre);
        m_tableauxPages[nomPeintre] = new QWidget(this);
        m_tableauxPages[nomPeintre]->setAccessibleName(nomPeintre);
        m_tableauxViewLayout[nomPeintre] = new QHBoxLayout;
        m_tableauxPages[nomPeintre]->setLayout(m_tableauxViewLayout[nomPeintre]);
        m_pages->addWidget(m_tableauxPages[nomPeintre]);

        QListIterator<RegulTableau*> itt(pepe->Tableaux());
        while (itt.hasNext()) {
            RegulTableau *tata = itt.next();
            QString nomTableau = tata->nom();
            QAction *tableau   = new QAction(tableauIco, nomTableau);
            connect(tableau, &QAction::triggered, [=](){ RegulMainWindow::showTableau(nomPeintre + ":" + nomTableau);});
            menuPeintres->addAction(tableau);
        }
    }
    m_menuFile = menuBar()->addMenu(tr("&Tracé"));
}

///////////////////////////////////////////////////////////////////////////////////////////
void  RegulMainWindow::displayAuthorsDirectory()
{
    QStringList images = m_imagesDir.entryList(QStringList() << "*.jpg", QDir::Files);
    if (images.isEmpty()) {
        m_authorsViewLayout->addWidget(m_infoLabel);
    } else
        foreach(QString fileName, images)
            displayOneAuthor(fileName);
    m_statusBarLabel->setText("Les peintres");
    adjustSize();
}

///////////////////////////////////////////////////////////////////////////////////////////
void  RegulMainWindow::displayTableauxDirectory(const QString &auteur)
{
    QDir imagesDir(m_imagesDir.path() + "/" + auteur);
    QStringList images = imagesDir.entryList(QStringList() << "*LD.jpg", QDir::Files);
    foreach(QString fileName, images)
        displayOneTableau(auteur, fileName);
    adjustSize();
}

///////////////////////////////////////////////////////////////////////////////////////////
void  RegulMainWindow::displayOneAuthor(const QString& fileName)
{
    QString auteur = fileName.left(fileName.lastIndexOf("."));
    QString nomComplet = m_academie->nomComplet(auteur);

    RegulLabel *imageTex = new RegulLabel(nomComplet);
    RegulPushButton *image = new RegulPushButton;
    image->setImageFileName(m_imagesDir.path() + "/" + auteur);
    connect(image, &QPushButton::pressed, [=](){setPage(auteur);});

    QVBoxLayout *imageLayout = new QVBoxLayout();
    imageLayout->setSpacing(1);
    imageLayout->addWidget(image, 0, Qt::AlignHCenter);
    imageLayout->addWidget(imageTex, 0, Qt::AlignHCenter);

    m_infoLabel->hide();
    m_authorsViewLayout->addLayout(imageLayout);
}

///////////////////////////////////////////////////////////////////////////////////////////
void  RegulMainWindow::displayOneTableau(const QString &auteur, const QString &fileName)
{
    QString tableau = fileName.left(fileName.lastIndexOf("_"));

    QDir imagesDir(m_imagesDir.path() + "/" + auteur);

    RegulLabel *imageTex    = new RegulLabel(tableau);
    RegulPushButton *image = new RegulPushButton(m_tableauxPages[auteur]);
    image->setAccessibleName(tableau);
    image->setImageFileName(imagesDir.path() + "/" + fileName);

    connect(image, &QPushButton::pressed, [=](){viewImage(image->imageFileName());});

    QVBoxLayout *imageLayout = new QVBoxLayout;
    imageLayout->addWidget(image, 0, Qt::AlignHCenter);
    imageLayout->addWidget(imageTex, 0, Qt::AlignHCenter);

    m_tableauxViewLayout[auteur]->addLayout(imageLayout);
}

///////////////////////////////////////////////////////////////////////////////////////////
void  RegulMainWindow::print()
{
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec()) {
        QPainter painter(&printer);
        painter.setRenderHint(QPainter::Antialiasing);
        m_imager->theView()->scene()->render(&painter);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void  RegulMainWindow::restore()
{
    QString desktopDir = QStandardPaths::displayName(QStandardPaths::DesktopLocation);
    desktopDir.prepend(QDir::homePath() +"/");
    QFile inFile(QFileDialog::getOpenFileName(this, "Restaure tracé", desktopDir, "*.tra"));
    if (inFile.fileName().isEmpty())
        return;
    inFile.open(QIODevice::ReadOnly);
    QDataStream in(&inFile);
    QList<QGraphicsItem *> items;
    QString auteur, tableau;
    in >> auteur >> tableau;
    if (auteur != m_currentAuteur || tableau != m_currentTableau) {
        QMessageBox::information(this, "Restauration tracé", "Ce tracé ne correspond pas au tableau affiché!");
        return;
    }
    RegulScene *scene = dynamic_cast<RegulScene*>(m_imager->theView()->scene());
    RegulItemGroup * carreaux = nullptr;
    RegulItemGroup * diagonales = nullptr;
    int type;
    while (!in.atEnd()) {
        in >> type;
        switch (type) {
        case RegulLineItem::Type: {
            RegulLineItem *item = new RegulLineItem;
            in >> item;
            items << item;
            scene->addHandDrawnLine(item);
            break;
        }
        case RegulLineItem::TypeC: {
            RegulLineItem *item = new RegulLineItem;
            in >> item;
            items << item;
            if (!carreaux)
                carreaux = scene->setCarreaux();
            carreaux->addToList(item);
//            scene->addHandDrawnLine(item);
            break;
        }
        case RegulLineItem::TypeD: {
            RegulLineItem *item = new RegulLineItem;
            in >> item;
            items << item;
            if (!diagonales)
                diagonales = scene->setDiagonales();
            diagonales->addToList(item);
//            scene->addHandDrawnLine(item);
            break;
        }
        case RegulEllipseItem::Type: {
            RegulEllipseItem *item = new RegulEllipseItem;
            in >> item;
            items << item;
            scene->addHandDrawnEllipse(item);
            break;
        }
//        case QGraphicsItemGroup::Type: {
//            QGraphicsItemGroup *group = new QGraphicsItemGroup;
//            int mytype;
//            in >> mytype;
//            if (mytype == RegulLineItem::Type) {
//                RegulLineItem *item = new RegulLineItem;
//                in >> item;
//                group->addToGroup(item);
//            }
//            items << group;
//            break;
//        }
//        case RegulItemGroup::Type: {
//            RegulItemGroup *group = nullptr;
//            qsizetype size;
//            RegulItemGroup::SUBTYPE subType;
//            in >> subType;
//            if (subType == RegulItemGroup::kCarreaux)
//                group = scene->setCarreaux();
//            else if (subType == RegulItemGroup::kDiagonales)
//                group = scene->setDiagonales();
//            else
//                qFatal("subtype de RegulItemGroup inconnu");
//            in >> size;
//            for (qsizetype index = 0; index < size; index++) {
//                RegulLineItem *item = new RegulLineItem;
//                in >> item;
//                group->addToGroup(item);
//            }
//            items << group;
//            break;
//        }
        default:
            break;
        }
    }
    for(QGraphicsItem *item : items){
        m_imager->theView()->scene()->addItem(item);
    }
    inFile.close();
}

///////////////////////////////////////////////////////////////////////////////////////////
void  RegulMainWindow::returnFirstPage()
{
    m_pages->setCurrentIndex(0);
    m_aRetour->setVisible(false);
    m_statusBarLabel->setText("Les Peintres");
}

///////////////////////////////////////////////////////////////////////////////////////////
void  RegulMainWindow::save()
{
    QString desktopDir = QStandardPaths::displayName(QStandardPaths::DesktopLocation);
    desktopDir.prepend(QDir::homePath() +"/");
    QString name = m_currentAuteur + "_" + m_currentTableau + ".tra";
    name.prepend(desktopDir +"/");
    QFile ouFile(QFileDialog::getSaveFileName(this, "sauvegarde tracé", name, "*.tra"));
    if (ouFile.fileName().isEmpty())
        return;
    ouFile.open(QIODevice::WriteOnly);
    QDataStream out(&ouFile);
    RegulScene *scene = dynamic_cast<RegulScene*>(m_imager->theView()->scene());
    out << m_currentAuteur << m_currentTableau;
    for (QGraphicsItem * item : scene->items()) {
        int type = item->type();
        switch (type) {
        case RegulLineItem::Type: {
            RegulLineItem * myItem = dynamic_cast<RegulLineItem*>(item);
            if (myItem->isHandDrawn()) {
                out << type;
                out << myItem;
            }
            break;
        }
        case RegulLineItem::TypeC: {
            RegulLineItem * myItem = dynamic_cast<RegulLineItem*>(item);
            if (myItem->isHandDrawn()) {
                out << type;
                out << myItem;
            }
            break;
        }
        case RegulLineItem::TypeD: {
            RegulLineItem * myItem = dynamic_cast<RegulLineItem*>(item);
            if (myItem->isHandDrawn()) {
                out << type;
                out << myItem;
            }
            break;
        }
        case RegulEllipseItem::Type: {
                RegulEllipseItem * myItem = dynamic_cast<RegulEllipseItem*>(item);
                if (myItem->isHandDrawn()) {
                    out << type;
                    out << myItem;
                }
            break;
        }
        case QGraphicsItemGroup::Type: {
            out << type;
            QList<QGraphicsItem*> items = item->childItems();
            for(QGraphicsItem* it : items) {
                if (it->type() == RegulLineItem::Type) {
                    out << it->type();
                    out << dynamic_cast<RegulLineItem*>(it);
                }
            }
            break;
        }
//        case RegulItemGroup::Type: {
//            RegulItemGroup *gItem = dynamic_cast<RegulItemGroup*>(item);
//            QList<QGraphicsItem*> items = gItem->childItems();
//            qsizetype size = items.size();
//            out << type << gItem->subType() <<  size;
//            for(QGraphicsItem* it : items)
//                    out << dynamic_cast<RegulLineItem*>(it);
//            break;
//        }
        default:
            break;
        }
    }
    ouFile.close();
    m_imager->setTraceSaved(true);
}

///////////////////////////////////////////////////////////////////////////////////////////
void  RegulMainWindow::setPage(const QString& auteur)
{
    m_pages->setCurrentWidget(m_tableauxPages[auteur]);
    m_aRetour->setVisible(true);
    m_statusBarLabel->setText("Les tableaux de " + auteur);
}

///////////////////////////////////////////////////////////////////////////////////////////
void  RegulMainWindow::showTableau(const QString &name)
{
    m_downloadProgress->show();
    m_academie->setCurrent(name);
    m_academie->setImagesAndPhotoLoaded(false);
    m_academie->downLoadImagesAndPhoto();
}

///////////////////////////////////////////////////////////////////////////////////////////
void  RegulMainWindow::updatePagePeintre()
{
    displayOneAuthor(m_academie->peintreCourant() +".jpg");
}

///////////////////////////////////////////////////////////////////////////////////////////
void  RegulMainWindow::updatePageTableaux()
{
    displayOneTableau(m_academie->peintreCourant(), m_academie->tableauCourant() +"_LD.jpg");
    m_downloadProgress->hide();
}

///////////////////////////////////////////////////////////////////////////////////////////
void  RegulMainWindow::viewImage(const QString &tableau)
{
    QString tableauHD = tableau;
    tableauHD.remove("_LD");
    m_currentTableau = tableauHD.last(tableauHD.size() - tableauHD.lastIndexOf("/") - 1);
    m_currentTableau = m_currentTableau.first(m_currentTableau.indexOf("."));
    m_currentAuteur = m_pages->currentWidget()->accessibleName();

    qreal width, height;
    m_academie->tailleTableau(m_currentAuteur, m_currentTableau, width, height);

    QGuiApplication::setApplicationDisplayName("nomTableau");
    m_imager->init();
    m_imager->setRealSize(width, height);
    m_imager->loadImage(tableauHD);

    if (m_menuFile->isEmpty()) {
        QIcon saveIco = QIcon(":/icons/save.png");
        m_menuFile->addAction(saveIco, tr("Sauvegarde"), this, &RegulMainWindow::save);

        QIcon restoreIco = QIcon(":/icons/open.png");
        m_menuFile->addAction(restoreIco, tr("Restaure"), this, &RegulMainWindow::restore);

        QIcon printIco = QIcon(":/icons/print.png");
        m_menuFile->addAction(printIco, tr("Impression"), this, &RegulMainWindow::print);
    }
}
