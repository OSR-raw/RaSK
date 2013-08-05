# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = QtRaSK
DESTDIR = ../Win32/Debug
QT += core opengl widgets gui
CONFIG += debug
DEFINES += QT_DLL QT_WIDGETS_LIB QT_OPENGL_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug

LIBS += -L"./Libs" \
    -lopengl32 \
    -lglu32
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
HEADERS += ./Core/GraphiscOGL.h \
    ./Core/Input.h \
    ./Core/Loader.h \
    ./Core/main.h \
    ./Core/Raycaster.h \
    ./Core/Render.h \
    ./Core/RTT.h \
    ./Core/Shader.h \
    ./Core/Texture2D.h \
    ./Core/Texture3D.h \
    ./Core/VAO.h \
    ./Core/VBO.h \
    ./qtrask.h \
    ./brush.h \
    ./paintarea.h \
    ./oglwidget.h
SOURCES += ./brush.cpp \
    ./Core/Input.cpp \
    ./Core/Loader.cpp \
    ./Core/Raycaster.cpp \
    ./Core/Render.cpp \
    ./Core/RTT.cpp \
    ./Core/Shader.cpp \
    ./Core/Texture2D.cpp \
    ./Core/Texture3D.cpp \
    ./Core/VAO.cpp \
    ./Core/VBO.cpp \
    ./main.cpp \
    ./oglwidget.cpp \
    ./paintarea.cpp \
    ./qtrask.cpp
FORMS += ./qtrask.ui
RESOURCES += qtrask.qrc
