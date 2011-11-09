QT       += core \
        gui \
        sql \
        xml \

INCLUDEPATH += include \
               "C:/dev/NCReport_minGW/include"


LIBS += -L"c:/dev/NCReport_minGW/lib" -lncreport2 \

TARGET = app
TEMPLATE = app


SOURCES += main.cpp mainform.cpp frmclients.cpp sqlmodules.cpp frmsotr.cpp frmuslugi.cpp \
    frm_okazanie_uslug.cpp \
    MyModel.cpp \
    frmselect.cpp \
    frmschetclienta.cpp \
    frmspr.cpp \
    include/qdatelineedit.cpp \
    include/qsellineedit.cpp \
    include/procedures.cpp \
    frm_setting.cpp \
    frmdocument.cpp \
    print.cpp \
    srcReports/repsklad.cpp


HEADERS  += mainform.h sqlmodules.h frmclients.h frmsotr.h MyModel.h frmuslugi.h \
    frm_okazanie_uslug.h \
    frmselect.h \
    include/delegats.h \
    frmschetclienta.h \
    frmspr.h \
    include/qdatelineedit.h \
    include/qsellineedit.h \
    include/procedures.h \
    frm_setting.h \
    frmdocument.h \
    params.h \
    print.h \
    srcReports/repsklad.h

FORMS    += mainform.ui \
    frmclients.ui \
    frmsotr.ui \
    frmuslugi.ui \
    frm_okazanie_uslug.ui \
    frmselect.ui \
    frmschetclienta.ui \
    frmspr.ui \
    frm_setting.ui \
    frmdocument.ui \
    srcReports/repsklad.ui

RESOURCES += \
    icons.qrc







































































