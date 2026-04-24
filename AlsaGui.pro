QT += widgets
TARGET = AlsaGui
TEMPLATE = app
SOURCES += main.cpp AlsaManager.cpp
HEADERS += AlsaManager.h
LIBS += -lasound
