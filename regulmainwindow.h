#ifndef REGULMAINWINDOW_H
#define REGULMAINWINDOW_H

#include <QLayout>
#include <QMainWindow>
#include <QStackedWidget>

#include "regulacademie.h"
#include "downloadmanager.h"
#include "regulimager.h"
#include "regullabel.h"
#include "regultableau.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RegulMainWindow; }
QT_END_NAMESPACE

class  RegulMainWindow : public QMainWindow
{
    Q_OBJECT

public:
     RegulMainWindow(QWidget *parent = nullptr);
    ~ RegulMainWindow() override;

     DownLoadManager* downLoadManager() const { return m_mgr;}
     QString          pathName() const        { return m_kPath;}

public slots:
    void displayOneAuthor(const QString& fileName);
    void displayOneTableau(const QString& auteur, const QString& fileName);
    void print();
    void restore();
    void save();
    void setPage(const QString & auteur);
    void returnFirstPage();
    void updatePagePeintre();
    void updatePageTableaux();
    void viewImage(const QString &tableau);

private:
    void showTableau(const QString& name);
    void buildMenu();
    void displayAuthorsDirectory();
    void displayTableauxDirectory(const QString& auteur);

    Ui:: RegulMainWindow *ui;

    RegulAcademie*                 m_academie;
    QAction*                       m_aRetour;
    QWidget*                       m_authorsPage;
    QHBoxLayout*                   m_authorsViewLayout;
    QString                        m_currentAuteur;
    QString                        m_currentTableau;
    QLabel*                        m_downloadProgress;
    QDir                           m_imagesDir;
    RegulImager*                   m_imager;
    RegulLabel*                    m_infoLabel;
    DownLoadManager*               m_mgr;
    QMenu*                         m_menuFile;
    QStackedWidget*                m_pages;
    QLabel*                        m_statusBarLabel;
    QMap<QString, QWidget*>        m_tableauxPages;
    QMap<QString, QHBoxLayout*>    m_tableauxViewLayout;
    QDir                           m_texDir;
    const QString                  m_kPath = "/tmp/RegulArt/";

};
#endif // REGULMAINWINDOW_H
