#include "dreamnotebook.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DreamNotebook w;
    w.show();
    return a.exec();
}
