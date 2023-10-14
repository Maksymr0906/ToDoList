#include "ToDoList.hpp"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ToDoList mainWindow;
    mainWindow.setFixedSize(800, 600);
    mainWindow.show();
    return a.exec();
}
