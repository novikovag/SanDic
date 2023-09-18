QT += core widgets sql

TARGET = sandic
TEMPLATE = app

SOURCES += main.cpp \
  mainwindow.cpp \
  card.cpp \
  config.cpp \
  mainwidget.cpp \
  trans.cpp \
  orderdialog.cpp

HEADERS  += \
  mainwindow.h \
  card.h \
  config.h \
  mainwidget.h \
  trans.h \
  orderdialog.h

FORMS += \
  mainwidget.ui \
  orderdialog.ui

RESOURCES += \
  sandic.qrc

RC_FILE = rc/sandic.rc

# Для Мака, на данный момент не нужно
# CONFIG -= app_bundle
CONFIG += c++20

