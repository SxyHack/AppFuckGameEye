QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += \
    themes/dark/framelesswindow \
    themes/dark \
    src/

SOURCES += \
    themes/dark/DarkStyle.cpp \
    themes/dark/framelesswindow/framelesswindow.cpp \
    themes/dark/framelesswindow/windowdragger.cpp \
    src/log/CustomLogMessageHandler.cpp \
    src/settings/GlobalSettings.cpp \
    MainWindow.cpp \
    main.cpp \

HEADERS += \
    themes/dark/DarkStyle.h \
    themes/dark/framelesswindow/framelesswindow.h \
    themes/dark/framelesswindow/windowdragger.h \
    src/utility/utility.h \
    src/version/version.auto.h \
    MainWindow.h

FORMS += \
    MainWindow.ui \
    themes/dark/framelesswindow/framelesswindow.ui \

RESOURCES += \
themes/dark/darkstyle.qrc \
themes/dark/framelesswindow.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CONFIG(debug, debug|release) {
    DESTDIR = builds/debug
} else {
    DESTDIR = builds/release
}
