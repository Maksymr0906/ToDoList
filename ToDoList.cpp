#include "ToDoList.hpp"

ToDoList::ToDoList(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("todolist");
    db.setUserName("postgres");
    db.setPassword("12345");

    if(db.open()) {
        QSqlQuery query;
        query.prepare("SELECT task_name FROM todolist");
        if(query.exec()) {
            while(query.next()) {
                QString column1Value = query.value(0).toString();
                QMessageBox::information(this, "Query", column1Value);
            }
        }
        else {
            QMessageBox::warning(this, "Query fails", "Query execution failed");
        }
    }
    else {
        QMessageBox::warning(nullptr, "DB Open Failed", db.lastError().text());
    }
}

ToDoList::~ToDoList()
{}
