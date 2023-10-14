#include "ToDoList.hpp"
#include <QtWidgets/QApplication>

//додати, що коли сьогоднішня дата, то воно автоматично додає в мій день
//переробити дата валідатор, щоб я міг не вводити дані
//додати імейл валідатор
//якщо дата менше теперішньої позначати як фейлед

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ToDoList mainWindow;
    mainWindow.setFixedSize(800, 600);
    mainWindow.show();
    return a.exec();
}
