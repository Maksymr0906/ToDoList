#include "ToDoList.hpp"
#include <QTranslator>
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load("lang_en.qm");
    a.installTranslator(&translator);
    ToDoList mainWindow;
    mainWindow.setFixedSize(800, 600);
    mainWindow.show();
    return a.exec();
}
