QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    configfilewidget.cpp \
    licencedialog.cpp \
    main.cpp \
    mainwindowcssh.cpp

HEADERS += \
    configfilewidget.h \
    licencedialog.h \
    mainwindowcssh.h

FORMS += \
    configfilewidget.ui \
    licencedialog.ui \
    mainwindowcssh.ui

TRANSLATIONS += \
    cssh-windows_en_GB.ts \
    cssh-windows_fr_FR.ts \
    cssh-windows_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
