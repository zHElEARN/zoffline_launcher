QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/configmanager/configmanager.cpp \
    src/hosts/hosts.cpp \
    src/logger/logger.cpp \
    src/main.cpp \
    src/mainwindow/mainwindow.cpp \
    src/serverlistmanager/serverlistmanager.cpp \
    src/utils/utils.cpp

HEADERS += \
    src/configmanager/configmanager.h \
    src/hosts/hosts.h \
    src/logger/logger.h \
    src/logger/logger.tpp \
    src/mainwindow/mainwindow.h \
    src/serverlistmanager/serverlistmanager.h \
    src/utils/utils.h

FORMS += \
    ui/mainwindow.ui \
    ui/serverlistmanager.ui

RC_FILE += \
    resources/resources.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    resources/configure_client.bat \
    resources/resources.rc \
    resources/uac.manifest

RESOURCES += \
    resources/resources.qrc

INCLUDEPATH += \
    src/
