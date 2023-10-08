#include "ToDoList.hpp"

ToDoList::ToDoList(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::ToDoListClass),
      dateValidator{ new DateValidator(this) } {
    ui->setupUi(this);
    ui->refreshBtn->setVisible(false);
    ui->refreshBtn->setEnabled(false);
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("todolist");
    db.setUserName("postgres");
    db.setPassword("12345");

    if(!db.open())
        QMessageBox::critical(this, "Error", "Error opening database");

    mainModel = new QSqlTableModel(this);
    mainModel->setTable("todolist");
    mainModel->select();

    tableView = new TasksTableView(mainModel, this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(tableView);
    ui->listTasksFrame->setLayout(layout);

    // connects
    connect(ui->actionAdd, &QAction::triggered, this, &ToDoList::actionAddTriggered);
    connect(ui->addTaskButton, SIGNAL(clicked()), this, SLOT(actionAddTriggered()));
    connect(ui->actionRemove, &QAction::triggered, this, &ToDoList::actionRemoveTriggered);
    connect(ui->removeTaskButton, SIGNAL(clicked()), this, SLOT(actionRemoveTriggered()));
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
    const int IN_PROCESS = 0;
    std::unique_ptr<NewTaskDialog> newTaskDialog = std::make_unique<NewTaskDialog>(dateValidator);
    newTaskDialog->setFixedSize(317, 255);
    if(newTaskDialog->exec() == QDialog::Accepted) {
        QSqlQuery insertQuery;
        QString insertQueryString = "insert into todolist(task_name, deadline, status, is_important, is_my_day)"
            "values('%1', '%2', '%3', '%4', '%5');";
        bool insertResult = insertQuery.exec(insertQueryString.arg(newTaskDialog->getTaskName())
                                             .arg(newTaskDialog->getDeadline())
                                             .arg(IN_PROCESS)
                                             .arg(newTaskDialog->getIsImportant())
                                             .arg(newTaskDialog->getIsMyDay())
        );
        if(!insertResult) {
            QMessageBox::critical(this, tr("Error"), tr("Adding task error"));
            return;
        }
    }
}

void ToDoList::actionRemoveTriggered() {
    
}

void ToDoList::actionMyDayTriggered() {
   
}

void ToDoList::actionImportantTriggered() {

}

void ToDoList::actionAllTriggered() {
    
}

void ToDoList::actionPlannedTriggered() {
    
}

void ToDoList::actionCompletedTriggered() {
    
}

void ToDoList::actionFailedTriggered() {
    
}

void ToDoList::actionAboutProgramTriggered() {
    QMessageBox::information(this, "About Program", "Action About Program triggered");
}

void ToDoList::refreshTitle(TASK_TYPE taskType) {
    refreshTitleText(taskType);
    refreshTitleIcon(taskType);
}

void ToDoList::refreshTitleText(TASK_TYPE taskType) {
    static const QMap<TASK_TYPE, QString> taskTypeTotitleText = {
        {TASK_TYPE::MY_DAY, "My Day"},
        {TASK_TYPE::IMPORTANT, "Important"},
        {TASK_TYPE::ALL, "All"},
        {TASK_TYPE::PLANNED, "Planned"},
        {TASK_TYPE::COMPLETED, "Completed"},
        {TASK_TYPE::FAILED, "Failed"}
    };

    if(taskTypeTotitleText.contains(taskType)) {
        ui->titleText->setText(taskTypeTotitleText.value(taskType));
    }
}

void ToDoList::refreshTitleIcon(TASK_TYPE taskType) {
    static const QMap<TASK_TYPE, QString> taskTypeToIcon = {
        {TASK_TYPE::MY_DAY, "Assets/myday_icon.png"},
        {TASK_TYPE::IMPORTANT, "Assets/important_icon.png"},
        {TASK_TYPE::ALL, "Assets/all_icon.png"},
        {TASK_TYPE::PLANNED, "Assets/planned_icon.png"},
        {TASK_TYPE::COMPLETED, "Assets/done_icon.png"},
        {TASK_TYPE::FAILED, "Assets/failed_icon.png"}
    };

    if(taskTypeToIcon.contains(taskType)) {
        ui->titleImage->setPixmap(taskTypeToIcon.value(taskType));
    }
}