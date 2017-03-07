QT += core gui multimedia multimediawidgets
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mylabel.cpp \
    mycamera.cpp

HEADERS += \
    mainwindow.h \
    mylabel.h \
    mycamera.h

FORMS +=

QMAKE_LFLAGS_WINDOWS += -Wl,--stack,32000000

