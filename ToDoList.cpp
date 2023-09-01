#include "ToDoList.hpp"

ToDoList::ToDoList(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::ToDoListClass)
{
    ui->setupUi(this);
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

    connect(ui->actionAdd, &QAction::triggered, this, &ToDoList::actionAddTriggered);
    connect(ui->actionEdit, &QAction::triggered, this, &ToDoList::actionEditTriggered);
    connect(ui->actionRemove, &QAction::triggered, this, &ToDoList::actionRemoveTriggered);
    connect(ui->actionMyDay, &QAction::triggered, this, &ToDoList::actionMyDayTriggered);
    connect(ui->actionImportant, &QAction::triggered, this, &ToDoList::actionImportantTriggered);
    connect(ui->actionAll, &QAction::triggered, this, &ToDoList::actionAllTriggered);
    connect(ui->actionPlanned, &QAction::triggered, this, &ToDoList::actionPlannedTriggered);
    connect(ui->actionCompleted, &QAction::triggered, this, &ToDoList::actionCompletedTriggered);
    connect(ui->actionFailed, &QAction::triggered, this, &ToDoList::actionFailedTriggered);


    connect(ui->actionAboutProgram, &QAction::triggered, this, &ToDoList::actionAboutProgramTriggered);
}

ToDoList::~ToDoList() {
    delete ui;
}

void ToDoList::actionAddTriggered() {
    QMessageBox::information(this, "Add", "Action Add triggered");
}

void ToDoList::actionEditTriggered() {
    QMessageBox::information(this, "Edit", "Action Edit triggered");
}

void ToDoList::actionRemoveTriggered() {
    QMessageBox::information(this, "Remove", "Action Remove triggered");
}

void ToDoList::actionMyDayTriggered() {
    QMessageBox::information(this, "My Day", "Action My day triggered");
}

void ToDoList::actionImportantTriggered() {
    QMessageBox::information(this, "Important", "Action Important triggered");
}

void ToDoList::actionAllTriggered() {
    QMessageBox::information(this, "All", "Action All triggered");
}

void ToDoList::actionPlannedTriggered() {
    QMessageBox::information(this, "Planned", "Action Planned triggered");
}

void ToDoList::actionCompletedTriggered() {
    QMessageBox::information(this, "Completed", "Action Completed triggered");
}

void ToDoList::actionFailedTriggered() {
    QMessageBox::information(this, "Failed", "Action Failed triggered");
}

void ToDoList::actionAboutProgramTriggered() {
    QMessageBox::information(this, "About Program", "Action About Program triggered");
}