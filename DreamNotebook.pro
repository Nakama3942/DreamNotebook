QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
RC_ICONS = icon.ico
VERSION = 1.0.0.1
QMAKE_TARGET_COMPANY = Kalynovsky Valentin
QMAKE_TARGET_PRODUCT = DreamNotebook
QMAKE_TARGET_DESCRIPTION = A program for recording your dreams.
QMAKE_TARGET_COPYRIGHT = Copyright (C) 2021 Kalynovsky Valentin. All rights reserved.

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dreameditor.cpp \
    main.cpp \
    dreamnotebook.cpp

HEADERS += \
    dreameditor.h \
    dreamnotebook.h

FORMS += \
    dreameditor.ui \
	dreamnotebook.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
