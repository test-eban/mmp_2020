QT += quick multimedia core gui
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/gl/connectfour.cpp \
        src/gl/glboard.cpp \
        src/gl/glbody.cpp \
        src/gl/glbodygroup.cpp \
        src/gl/glcolorrgba.cpp \
        src/gl/glesrenderer.cpp \
        src/gl/glitem.cpp \
        src/gl/glmultiplebody.cpp \
        src/gl/glpoint.cpp \
        src/gl/gltoken.cpp \
        src/gl/gltokentray.cpp \
        src/main.cpp \
        src/gl/shaderdebugger.cpp \
        src/sound/soundengine.cpp

RESOURCES += qml.qrc \
    models.qrc \
    shaders.qrc \
    sounds.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/gl/connectfour.h \
    src/gl/glboard.h \
    src/gl/glbody.h \
    src/gl/glbodygroup.h \
    src/gl/glcolorrgba.h \
    src/gl/gldefines.h \
    src/gl/glesrenderer.h \
    src/gl/glitem.h \
    src/gl/glmultiplebody.h \
    src/gl/glpoint.h \
    src/gl/gltoken.h \
    src/gl/gltokentray.h \
    src/gl/shaderdebugger.h \
    src/sound/soundengine.h
