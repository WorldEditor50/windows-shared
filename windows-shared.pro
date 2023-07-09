TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        crashdump.cpp \
        main.cpp \
        process.cpp \
        winnative.cpp

HEADERS += \
    crashdump.h \
    library.hpp \
    process.h \
    winnative.h

LIBS += shell32.lib user32.lib
