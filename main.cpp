#include <QtGui/QApplication>
#include "mainform.h"

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("cp1251");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    QApplication a(argc, argv);
    a.setStyle("cleanlooks");
    QResource::registerResource(QDir::currentPath()+"/icons.rcc");
    a.addLibraryPath(QDir::currentPath()+"/plugins");
    MainForm w;
    w.show();

    return a.exec();
}
