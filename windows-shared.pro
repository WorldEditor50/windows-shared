TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        commandline.cpp \
        fileutils.cpp \
        main.cpp \
        process.cpp \
        regeditor.cpp \
        sharedmemory.cpp \
        stringutils.cpp

HEADERS += \
    clock.hpp \
    cmutex.hpp \
    commandline.h \
    common_def.h \
    conditionalvar.hpp \
    crashdump.hpp \
    criticalsection.hpp \
    csv.hpp \
    cthread.hpp \
    ctimer.hpp \
    daemon.hpp \
    fileutils.h \
    inihelper.hpp \
    library.hpp \
    pipe.h \
    process.h \
    regeditor.h \
    semaphore.hpp \
    service.hpp \
    sharedmemory.h \
    srwlock.hpp \
    stringutils.h \
    tcpserver.hpp

LIBS += shell32.lib user32.lib
