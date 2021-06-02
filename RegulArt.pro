QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    downloadmanager.cpp \
    main.cpp \
    regulacademie.cpp \
    regulellipseitem.cpp \
    regulimager.cpp \
    regulitemgroup.cpp \
    regullabel.cpp \
    regullineitem.cpp \
    regulmainwindow.cpp \
    regulpeintre.cpp \
    regulpushbutton.cpp \
    regulrectitem.cpp \
    regulscene.cpp \
    regulstreamers.cpp \
    regultableau.cpp \
    regultextitem.cpp \
    regultoolbox.cpp \
    regulviewer.cpp

HEADERS += \
    downloadmanager.h \
    regulacademie.h \
    regulellipseitem.h \
    regulimager.h \
    regulitemgroup.h \
    regullabel.h \
    regullineitem.h \
    regulmainwindow.h \
    regulpeintre.h \
    regulpushbutton.h \
    regulrectitem.h \
    regulscene.h \
    regulstreamers.h \
    regultableau.h \
    regultextitem.h \
    regultoolbox.h \
    regulviewer.h

FORMS += \
    regulmainwindow.ui

TRANSLATIONS += \
    ArtRegul_fr_FR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qt.qrc

DISTFILES += \
    icons/arrow-computer-icons-clip-art-red-arrow-left-curved-png-5619727bcc6183b86881941971e65639.png
