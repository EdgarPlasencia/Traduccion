#include "principal.h"

#include <QApplication>
#include <QTranslator>
#include <QInputDialog>
#include <QString>
#include <QStringList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator t;
    QStringList lenguajes;
    lenguajes<<"Spanish"<<"French"<<"Italian";

    QString lang=QInputDialog::getItem(NULL,"Seleccione un idioma",
                                       "Idioma",lenguajes);
    if(lang=="French"){
        t.load(":/Idiomas/french.qm");
    }else if(lang=="Italian"){
        t.load(":/Idiomas/italian.qm");
    }
    if(lang!="Spanish"){
        a.installTranslator(&t);
    }
    Principal w;
    w.show();
    return a.exec();
}
