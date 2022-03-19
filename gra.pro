HEADERS       = glwidget.h \
                window.h \
                mainwindow.h \
    cmesh.h \
    player.h \
    gameobject.h \
    cube.h \
    enemy.h \
    terrain.h \
    texturemanager.h
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp \
                mainwindow.cpp \
    cmesh.cpp \
    player.cpp \
    gameobject.cpp \
    cube.cpp \
    enemy.cpp \
    terrain.cpp \
    texturemanager.cpp

QT           += widgets

# install
target.path = ./gra
INSTALLS += target

DESTDIR = builds
OBJECTS_DIR = builds/objects

DISTFILES += \
    shaders/shader.fs \
    shaders/shader.vs \
    models/sheep.obj \
    builds/textures/grass.jpg \
    builds/textures/grass.jpg
