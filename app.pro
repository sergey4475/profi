QT       += core gui sql

INCLUDEPATH += include

TARGET = app
TEMPLATE = app


SOURCES += main.cpp mainform.cpp frmclients.cpp sqlmodules.cpp frmsotr.cpp frmuslugi.cpp \
    frm_okazanie_uslug.cpp \
    MyModel.cpp \
    frmselect.cpp \
    frmschetclienta.cpp \
    frmspr.cpp \
    frmo_sklad.cpp \
    include/qdatelineedit.cpp \
    include/qsellineedit.cpp \
    include/procedures.cpp \
    frmraspred.cpp \
    frm_setting.cpp


HEADERS  += mainform.h sqlmodules.h frmclients.h frmsotr.h MyModel.h frmuslugi.h \
    frm_okazanie_uslug.h \
    frmselect.h \
    include/delegats.h \
    frmschetclienta.h \
    frmspr.h \
    frmo_sklad.h \
    include/qdatelineedit.h \
    include/qsellineedit.h \
    include/procedures.h \
    frmraspred.h \
    frm_setting.h

FORMS    += mainform.ui \
    frmclients.ui \
    frmsotr.ui \
    frmuslugi.ui \
    frm_okazanie_uslug.ui \
    frmselect.ui \
    frmschetclienta.ui \
    frmspr.ui \
    frmo_sklad.ui \
    frmraspred.ui \
    frm_setting.ui

RESOURCES += \
    icons.qrc










































